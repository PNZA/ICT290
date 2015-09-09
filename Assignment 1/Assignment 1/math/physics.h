//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Physics routines
//
//=============================================================================
#if 1
#ifndef PHYSICS_H
#define PHYSICS_H
#include "../types/bound_box.h"
#include <vector>
#include "../types/world_geometry.h"
#include "../types/polygon.h"
#include <float.h>

//Shays world is around 3.14 x the size of the real world
#define GRAVITY -9.8 * 3.14
#define EPSILON 0.00001

enum CollisionType {
	CT_NONE = 0, CT_SURFACE, CT_EDGE, CT_POINT, CT_EMBEDDED
};

typedef struct
{
	CPolygon *collider;
	CPlane collide_plane;
	float collide_distance;
	CollisionType collide_type;

}SCollisionInfo;

typedef vector<SCollisionInfo> CollisionList;

/**
Returns the closest point on the line to the given point.
Returns true if the closest point IS one of the line points, false if not.
*/
inline CVector ClosestPointOnLine(const CVector& line_start, const CVector& line_end, const CVector& point, bool &edge)
{
	CVector& c = point - line_start;
	CVector& v = line_end - line_start;
	v.Normalise();

	float t = v.DotProduct(c);
	
	edge = false;
	if(t < 0)
	{
		return line_start;
	}

	float d = line_end.DistanceTo(line_start);

	if(t > d)
	{
		return line_end;
	}

	edge = true;
	return line_start + v * t;
}

/**
Returns the closest point on a polygon's perimeter to the given point.
*/
inline CVector ClosestPointOnPolygonPerimeter(const CPolygon& poly, const CVector &p, CVector &line_start, CVector &line_end, bool &edgeFlag)
{
	bool found = false;
	float closestDistance = 0;
	CVector closestPoint = CVector(0,0,0);
	CVector closestP0, closestP1;
	int	closestIndex;

	CVector p0 = poly.GetVertex(0);

	int index = 0;
	vector<CVector>::const_iterator i = poly.GetVerticesBegin();
	i++;
	index++;

	while(i != poly.GetVerticesEnd())
	{
		CVector p1 = *i;
		bool edge;

		CVector cp = ClosestPointOnLine(p0, p1, p, edge);
		float d = cp.DistanceTo(p);

		if (!found || d < closestDistance)
		{
			closestDistance = d;
			closestPoint = cp;
			closestP0 = p0;
			closestP1 = p1;
			edgeFlag = edge;
			closestIndex = index;
			found = true;
		}

		p0 = p1;

		i++, index++;
	}

	if (!edgeFlag)
	{
		int	a = closestIndex - 1; if (a < 0) a = poly.GetNumVertices() - 1;
		int	b = closestIndex + 1; if (b >= poly.GetNumVertices()) b = 0;
		line_start = poly.GetVertex(a);
		line_end = poly.GetVertex(b);
	}
	else
	{
		line_start = closestP0;
		line_end = closestP1;
	}

	return closestPoint;
}

/**
Checks whether a point is on the inside of the given polygon.
http://www.paulnettle.com/
TODO move all of this inside the polygon class and make it generate its own collision planes
*/
inline bool PointInsidePolygon(const CVector &p, float epsilon, const CPolygon& poly)
{
	int	pos = 0;
	int	neg = 0;

	vector<CVector>::const_iterator j = poly.GetVerticesBegin();
	++j;
	if (j == poly.GetVerticesEnd()) j = poly.GetVerticesBegin();
	for (vector<CVector>::const_iterator i = poly.GetVerticesBegin(); i != poly.GetVerticesEnd(); ++i, ++j)
	{
		if (j == poly.GetVerticesEnd()) j = poly.GetVerticesBegin();

		// The current edge

		const CVector& p0 = *i;
		const CVector& p1 = *j;

		// Generate a normal for this edge
		CVector normal;
		(p1 - p0).CrossProduct(poly.GetCachedNormal(), normal);

		// Which side of this edge-plane is the point?

		float halfPlane = p.DotProduct(normal) - p0.DotProduct(normal);

		// Keep track of positives & negatives (but not zeros -- which means it's on the edge)

		if (halfPlane > epsilon) pos++;
		else if (halfPlane < -epsilon) neg++;
	}

	// If they're ALL positive, or ALL negative, then it's inside

	if (!pos || !neg) return true;

	// Must not be inside, because some were pos and some were neg

	return false;
}
inline void GetPotentialWorldColliders(const CBoundBox& box1, vector<CPolygon*>& colliders)
{
	GeometryManager& geo_manage = GeometryManager::GetInstance();
	int i, j, a;
	for(i = 0; i < geo_manage.ActiveAreas().size(); i++)
	{
		for(j = 0; j < geo_manage.GeometryInAreaCount(i); j++)
		{
			CWorldGeometry& geometry = geo_manage.GetGeometry(i, j);

			if(box1.IsIntersectingBox(geometry.GetBoundBox()) || box1.IsInsideBox(geometry.GetBoundBox()))
			{
				for(a = 0; a < geometry.GetSurfaceCount(); a++)
				{
					//collect pointers to all the potential colliders (as polygons)
					//cout << geometry.GetSurface(a).GetCachedMidpoint() << "\n";
					colliders.push_back((CPolygon*)&(geometry.GetSurface(a)));
				}
			}		
		}
	}
}

inline bool SphereIsEmbedded(const CPolygon& poly, const CVector& sphereCenter, CVector& innerMostPoint)
{
	// How far is the sphere from the plane?
	CPlane p = poly.GetPlane();

	float t = p.Distance(sphereCenter);

	// If the plane is farther than the radius of the sphere, it's not embedded

	if (t > 1 - EPSILON) return false;

	// Find the closest point on the polygon to the center of the sphere

	innerMostPoint = sphereCenter - p.GetNormal() * t;

	// If the closest point on the plane is within the polygon, the polygon is embedded

	if (PointInsidePolygon(innerMostPoint, EPSILON, poly))
	{
		CVector e0, e1;
		bool ef;
		innerMostPoint = ClosestPointOnPolygonPerimeter(poly,innerMostPoint, e0, e1, ef);
		if (innerMostPoint.DistanceTo(sphereCenter) > 1 - EPSILON) return false;
	}

	return true;
}
/**
Intersects a ray with a plane. Returns how far along the ray that the intersect takes place.
returns FLT_EPSILON if there is no intersect (ray in plane).
*/
inline float RayPlaneIntersect(const CVector& p_origin, const CVector& p_normal, const CVector& r_origin, const CVector& r_direction)
{
	if(float denom = r_direction.DotProduct(p_normal) == 0)
		return FLT_EPSILON; //if the ray is contained within the plane given

	return ((p_origin - r_origin).DotProduct(p_normal)) / r_direction.DotProduct(p_normal);

}

/**
Intersects a ray with a sphere. Returns how far along the ray intersects the sphere first.
returns FLT_EPSILON if there is no intersect.
*/
inline float RaySphereIntersect(const CVector& s_origin, float s_radius, const CRay& ray)
{
	//o-c
	CVector sphere_to_ray_origin = ray.GetOrigin() - s_origin;

	//l . (o-c)
	float p1 = ray.GetNormal().DotProduct(sphere_to_ray_origin);
	
	//(l . (o - c))^2 - ||o-c||^2 + r^2
	float p3 = (p1*p1) - sphere_to_ray_origin.GetSQLength() + s_radius*s_radius;

	printf("%f\n", p3);

	if(p3 < 0)
	{
		return FLT_EPSILON; //if this part is less than 0, there is no intersect.
	}

	return -p1 - sqrt(p3); //return the FIRST intersection point
}

inline bool UnitSphereIntersect(const CVector& center, const CRay& ray, float &time)
{
	CVector q = center - ray.GetOrigin();

	float c = q.GetLength();
	float v = q.DotProduct(ray.GetNormal());
	float d = 1 - (c*c - v*v);

	//Was there an intersection
	if (d < 0)
	{
		time = 0;
		return false;
	}

	//Return the distance to the first intersecting plane.
	time = v - sqrt(d);
	return true;
}

inline bool IsPointInsideEdge(const CVector& line_start, const CVector& line_end, const CVector& point)
{
	return(CVector(line_start-point).DotProduct(CVector(line_end-point)) <= 0);
}

#endif
#endif