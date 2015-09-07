
#define TEST_ENABLED 0

#if TEST_ENABLED

#include "types\world_geometry.h"
#include <stdlib.h>
#include <time.h>
#include "math\physics.h"
#include "types\world_geometry.h"

/*
void test_surface()
{
	CWorldGeometrySurface surface;
	CWorldGeometrySurface surface2;

	cout << surface.GetID() << "\n";
	cout << surface.GetTexture() << "\n";

	cout << surface2.GetID() << "\n";
	cout << surface2.GetTexture() << "\n";


	vector<CVector> verts;
	vector<tex_coord_t> tex_coords;

	verts.push_back(CVector(1,1,1));
	verts.push_back(CVector(1,3,5));
	verts.push_back(CVector(1,9,6));

	tex_coord_t coord;
	coord.u = 1;
	coord.v = 5;

	tex_coords.push_back(coord);

	coord.u = 5;
	coord.v = 10;

	tex_coords.push_back(coord);

	coord.u = 50;
	coord.v = 10;

	tex_coords.push_back(coord);

	CPolygon poly2 = CPolygon(verts, tex_coords);

	CWorldGeometrySurface surface3 = CWorldGeometrySurface(poly2, 100);

	cout << surface3.GetID() << "\n";
	cout << surface3.GetTexture() << "\n";

	cout << surface3.GetPolygon().GetNormal() << "\n";
}

void test_geometry()
{

}
*/

/*
Testing for physics intersect routines
*/
void test_line_plane_intersect()
{
	CVector ray_start = CVector(0,0,0);
	CVector ray_dir = CVector(0,1,0);

	CVector plane_point = CVector(0,0,0);
	CVector plane_normal = CVector(1,0,0);

	ray_dir.Normalise();
	plane_normal.Normalise();

	float intersect = RayPlaneIntersect(plane_point, plane_normal, ray_start, ray_dir);
	
	if(intersect == FLT_EPSILON)
	{
		cout << "There is no intersect\n";
	}
	else
	{
		CVector ray_collide;

		VectorMultiplyAdd(ray_start, intersect, ray_dir, ray_collide);
		cout << intersect << "\n";
		cout << ray_collide << "\n";
	}

}

void test_line_sphere_intersect()
{
	CVector ray_start = CVector(0,0,0);
	CVector ray_dir = CVector(0,-1,0);
	CVector sphere_origin = CVector(0,1100,0);
	ray_dir.Normalise();
	float sphere_radius = 1000;

	float intersect = RaySphereIntersect(sphere_origin, sphere_radius, ray_start, ray_dir);

	if(intersect == FLT_EPSILON)
	{
		cout << "There is no intersect\n";
	}
	else
	{
		CVector ray_collide;
		VectorMultiplyAdd(ray_start, intersect, ray_dir, ray_collide);
		cout << intersect << "\n";
		cout << ray_collide << "\n";
	}
}

void closest_point_on_line()
{
	CVector line_start = CVector(0,0,0);
	CVector line_end = CVector(0,0,10);
	CVector point = CVector(0,100,10);

	CVector result;

	ClosestPointOnLine(line_start, line_end, point, result);

	cout << result << "\n";
}

void point_inside_polygon()
{
	CVector point = CVector(-10001,-10000,-10000);
	CPolygon poly;

	poly.AddVertex(CVector(0,0,1));
	poly.AddVertex(CVector(0,1,0));
	poly.AddVertex(CVector(1,0,0));

	tex_coord_t temp = {0,0};
	poly.AddTexCoord(temp);
	poly.AddTexCoord(temp);
	poly.AddTexCoord(temp);

	poly.ComputeNormal();
	cout << poly.GetNormal() << "\n";

	cout << PointInsidePolygon(point, 0.00001, poly);
}

void closest_point_polygon_perimeter()
{
	CVector point = CVector(1,0,0);
	CPolygon poly;
	CVector result;

	poly.AddVertex(CVector(0,0,1));
	poly.AddVertex(CVector(0,1,0));
	poly.AddVertex(CVector(1,0,0));

	tex_coord_t temp = {0,0};
	poly.AddTexCoord(temp);
	poly.AddTexCoord(temp);
	poly.AddTexCoord(temp);

	poly.ComputeNormal();
	cout << poly.GetNormal() << "\n";

	ClosestPointOnPolygonPerimeter(point, poly, result);
	cout << result << "\n";
}

void main(void)
{	
	closest_point_polygon_perimeter();
	getchar();
}
#endif
