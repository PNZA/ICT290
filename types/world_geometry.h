//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements a world geometry surface. Static, unchanging.
//			Part of a larger CWorldGeometryObject
//
//=============================================================================
#include "IDrawable.h"
#include "polygon.h"
#include "vector.h"
#include "bound_box.h"

#ifndef CWORLD_GEOMETRY_H
#define CWORLD_GEOMETRY_H


/** 
Object representing a single surface of world geometry.
*/
class CWorldGeometrySurface
{
private:
	CPolygon m_polygon;
	unsigned int m_geometry_surface_id;
	unsigned int m_texture_id;

	unsigned int GetNextUniqueId();
public:
	CWorldGeometrySurface();
	CWorldGeometrySurface(CPolygon *polygon, unsigned int texture);

	void SetPolygon(CPolygon *polygon);
	CPolygon& GetPolygon();
	unsigned int GetID();
	unsigned int GetTexture();
	void SetTexture(unsigned int texture);
};

/** 
Object representing a set of world geometry surfaces that make up an object.
*/
class CWorldGeometry : IDrawable
{
private:
	CBoundBox m_boundbox;
	vector<CWorldGeometrySurface> m_surfaces;

	void BuildBoundBox();
	void AddToBoundBox(CWorldGeometrySurface& surface);
	unsigned int m_geometry_group;

public:
	CWorldGeometry();
	CWorldGeometry(vector<CWorldGeometrySurface> *surfaces, unsigned int group);

	void AddSurface(CWorldGeometrySurface& surface);
	unsigned int GetSurfaceCount();
	vector<CWorldGeometrySurface>::iterator GetSurfaceItr();

	const CBoundBox& GetBoundBox();

	void Draw();
	bool ShouldDraw();
};

#endif