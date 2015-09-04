//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: World geometry classes concerned with managing static world geometry.
//
//=============================================================================
#include "polygon.h"
#include "vector.h"
#include "bound_box.h"
#include <vector>
#include <list>

#ifndef CWORLD_GEOMETRY_H
#define CWORLD_GEOMETRY_H


//-----------------------------------------------------------------------------
//CWorldGeometrySurface
//-----------------------------------------------------------------------------
class CWorldGeometrySurface : public CPolygon
{
private:
	unsigned int m_geometry_surface_id;
	int m_texture_id;

	unsigned int GetNextUniqueId();
public:
	CWorldGeometrySurface();
	CWorldGeometrySurface(CPolygon& polygon, unsigned int texture);

	void init();

	int GetID();
	int GetTexture();
	void SetTexture(unsigned int texture);
};

//-----------------------------------------------------------------------------
//CWorldGeometry
//-----------------------------------------------------------------------------
class CWorldGeometry
{
private:
	CBoundBox m_boundbox;
	vector<CWorldGeometrySurface> m_surfaces;

	void BuildBoundBox();
	void AddToBoundBox(CWorldGeometrySurface& surface);
	unsigned int m_geometry_id;

public:
	CWorldGeometry();
	CWorldGeometry(vector<CWorldGeometrySurface>& surfaces);

	void AddSurface(CWorldGeometrySurface& surface);
	unsigned int GetSurfaceCount();
	const CWorldGeometrySurface& GetSurface(int i); 
	vector<CWorldGeometrySurface>::iterator GetSurfaceStart();
	vector<CWorldGeometrySurface>::iterator GetSurfaceEnd();

	unsigned int GetID(){return m_geometry_id;};

	const CBoundBox& GetBoundBox() const;

	void Compile(); // compiles world geometry into display lists for drawing later.

	//Also has
	void Draw();
	bool ShouldDraw();

	//for debug
	friend ostream& operator<<(ostream& os, const CWorldGeometry& geom)
	{
		os << "\nGeomID: " << geom.m_geometry_id << "\n";
		os << "BoundBox: " << "\n	Max: " << geom.m_boundbox.m_max << "\n	Min: " << geom.m_boundbox.m_min << "\n";
		os << "NumSurfaces: " << ((CWorldGeometry)geom).GetSurfaceCount() << "\n";
		
		int i, j;
		for(i = 0; i < geom.m_surfaces.size(); i++)
		{
			os << "	Texture: " << ((CWorldGeometrySurface)geom.m_surfaces[i]).GetTexture() << "\n";
			os << "	Normal: " << ((CPolygon)geom.m_surfaces[i]).GetNormal() << "\n";
			os << "	NumVerts: " << ((CPolygon)geom.m_surfaces[i]).GetNumVertices() << "\n";
			os << "	NumTexCoords: " << ((CPolygon)geom.m_surfaces[i]).GetNumTexCoords() << "\n";
			for(j = 0; j < ((CPolygon)geom.m_surfaces[i]).GetNumVertices(); j++)
			{
				os << "		" << ((CPolygon)geom.m_surfaces[i]).GetVertex(j) << "\n";
			}
		}
		return os;
	}
};

//-----------------------------------------------------------------------------
//GeometryManager
//
//Uses the singleton design pattern
//Based off pattern from http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
//-----------------------------------------------------------------------------
class GeometryManager
{
public:
	static GeometryManager& GetInstance()
	{
		static GeometryManager instance;
		return instance;
	}

	int AreaCreate(); //create a new area
	int AreaCount(); //number of areas

	bool IsAreaActive(int area_id); //Returns true if the specified area is active.

	void EnableArea(int area_id); //Enables the specified area
	void DisableArea(int area_id); //Disables the specified area

	int GeometryInAreaCount(int area_id); //number of geometry pieces in the area

	void AddGeometry(CWorldGeometry& geometry, int area_id); //adds a piece of geometry to the given area
	CWorldGeometry& GetGeometry(int area_id, int geo_id);

	const vector<int>& ActiveAreas()
	{
		return m_active_areas;
	}

	void CompileAll();

private:
	GeometryManager(){};
	vector<int> m_active_areas; 
	vector<vector<CWorldGeometry>> m_areas;

	int ActiveContainsArea(int area_id);

	//Don't permit people to make copies of this
	GeometryManager(GeometryManager const&);
	void operator=(GeometryManager const&);

};


/*
For holding information about textures to be pasted onto geometry.
*/
struct STexInfo
{
	float tex_scale_u;
	float tex_scale_v;

	float tex_offset_u;
	float tex_offset_v;

	int tex_id;
};

#define GEO_V_RIGHT true
#define GEO_V_LEFT false

#define GEO_H_UP true
#define GEO_H_DOWN false

class GeometryBuilder
{
public:
	GeometryBuilder();
	int NewArea();

	void StartObject();
	void StartSurface();

	void Texture(int tex_id);

	void TextureCoordinate(float u, float v);
	void TextureCoordinate(tex_coord_t& coord);

	void Vertex(float x, float y, float z);
	void Vertex(float* vert);
	void Vertex(CVector& vert);

	void EndSurface();
	void EndObject();

	void CreateVertical(const CVector& start, const CVector& end, float height, bool flip, const STexInfo& texture_info);
	void CreateHorizontal(const CVector& start, const CVector& end, float width, bool flip, const STexInfo& texture_info);
	void CreateStairs(const CVector& start, const CVector& end, int StepCount, const STexInfo& stair_top, const STexInfo& stair_side);

private:
	bool m_building_obj;
	bool m_building_surf;
	int m_curr_area;

	CWorldGeometry m_curr_obj;
	CWorldGeometrySurface m_curr_surf;

};
#endif