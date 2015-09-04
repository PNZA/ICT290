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

inline void GetPotentialWorldColliders(const CBoundBox& box1, vector<CWorldGeometry*>& colliders)
{
	GeometryManager& geo_manage = GeometryManager::GetInstance();
	int i, j, a;
	for(i = 0; i < geo_manage.ActiveAreas().size(); i++)
	{
		for(j = 0; j < geo_manage.AreaCount(); j++)
		{
			CWorldGeometry geometry = geo_manage.GetGeometry(i, j);
			if(box1.IsIntersectingBox(geometry.GetBoundBox()))
			{
				colliders.push_back(&geometry);
			}
				
		}
	}
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
inline float RaySphereIntersect(const CVector& s_origin, float s_radius, const CVector& r_origin, const CVector& r_direction)
{
	//o-c
	CVector sphere_to_ray_origin = r_origin - s_origin;

	//l . (o-c)
	float p1 = r_direction.DotProduct(sphere_to_ray_origin);
	
	//(l . (o - c))^2 - ||o-c||^2 + r^2
	float p3 = (p1*p1) - sphere_to_ray_origin.GetSQLength() + s_radius*s_radius;

	printf("%f\n", p3);

	if(p3 < 0)
	{
		return FLT_EPSILON; //if this part is less than 0, there is no intersect.
	}

	return -p1 - sqrt(p3); //return the FIRST intersection point
}

/**
Returns the closest point on the line to the given point.
*/
inline void ClosestPointOnLine(const CVector& line_start, const CVector& line_end, const CVector& point, CVector& closest)
{
	//a = line_start
	//b = line_end
	//p = point

	CVector V = line_end - line_start;
	float d = V.GetLength(); //distance from a to b.
	V.Normalise();
	float t = V.DotProduct(point - line_start);

	if (t < 0) 
	{
		closest = line_start;
		return;
	}

	if (t > d)
	{
		closest = line_end;
	}

	V *= t;
	closest = line_start + V;
}

/**
Returns the closest point on a polygon's perimeter to the given point.
*/
inline void ClosestPointOnPolygonPerimeter(const CVector& point, const CPolygon& polygon, CVector& closest)
{
	CVector temp = closest;
	int i;
	for(i = 0; i <= polygon.GetNumVertices(); i++)
	{
		if(i + 1 < polygon.GetNumVertices()) 
		{
			//Check the line between this vert and the previous one.
			ClosestPointOnLine(polygon.GetVertex(i), polygon.GetVertex(i+1), point, temp);
		}

		if(i == polygon.GetNumVertices())
		{
			//Check the line between the last vert and the first one.
			ClosestPointOnLine(polygon.GetVertex(0), polygon.GetVertex(i - 1), point, temp);

		}

		if((temp - point).GetSQLength() < (closest - point).GetSQLength())
		{
			//If the found point is closer than the current closest value, set closest to it.
			closest = temp;
		}
	}
}

/**
Checks whether a point is on the inside of the given polygon.
http://www.paulnettle.com/
*/
inline bool PointInsidePolygon(const CVector &p, float epsilon, const CPolygon& poly)
{
	if(!poly.IsValid()) return false;
	cout << "valid\n";
	int	pos = 0;
	int	neg = 0;


	int i;
	for(i = 0; i < poly.GetNumVertices(); i++)
	{
		// The current edge
		const CVector& p1 = poly.GetVertex(i);
		const CVector& p2 = p1;

		if(i+1 == poly.GetNumVertices())
		{
			const CVector& p2 = poly.GetVertex(0);
		}
		else
		{
			const CVector& p2 = poly.GetVertex(i+1);
		}

		// Generate a normal for this edge

		CVector normal;
		(p1 - p2).CrossProduct(poly.GetCachedNormal(), normal);
		cout << p1 << "\n";
		cout << p2 << "\n";
		cout << normal << "\n";

		// Which side of this edge-plane is the point?

		float halfPlane = (p.DotProduct(normal) - (p1.DotProduct(normal)));

		// Keep track of positives & negatives (but not zeros -- which means it's on the edge)

		if (halfPlane > epsilon) pos++;
		else if (halfPlane < -epsilon) neg++;
	}

	// If they're ALL positive, or ALL negative, then it's inside

	if (!pos || !neg) return true;

	// Must not be inside, because some were pos and some were neg

	return false;
}

#endif
#endif