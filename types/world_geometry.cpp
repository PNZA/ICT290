//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Functions for CWorldGeometrySurface class
//
//=============================================================================
#include "world_geometry.h"
#include <GL/freeglut.h>

//-----------------------------------------------------------------------------
//CWorldGeometrySurface
//-----------------------------------------------------------------------------
CWorldGeometrySurface::CWorldGeometrySurface()
{

}

CWorldGeometrySurface::CWorldGeometrySurface(CPolygon *polygon, unsigned int texture)
{
	m_polygon = *polygon;
	m_texture_id = texture;
}

void CWorldGeometrySurface::SetPolygon(CPolygon *polygon)
{
	m_polygon = *polygon;
}

CPolygon& CWorldGeometrySurface::GetPolygon()
{
	return m_polygon;
}

unsigned int CWorldGeometrySurface::GetID()
{
	return m_geometry_surface_id;
}

unsigned int CWorldGeometrySurface::GetTexture()
{
	return m_texture_id;
}

void CWorldGeometrySurface::SetTexture(unsigned int texture)
{
	m_texture_id = texture;
}

//-----------------------------------------------------------------------------
//CWorldGeometry
//-----------------------------------------------------------------------------
CWorldGeometry::CWorldGeometry()
{
	//initialise bound box to 0
	m_geometry_group = 0; //no group yet
}

CWorldGeometry::CWorldGeometry(vector<CWorldGeometrySurface> *surfaces, unsigned int group)
{
	m_surfaces = *surfaces;
	BuildBoundBox();
	m_geometry_group = group;
}

void CWorldGeometry::AddSurface(CWorldGeometrySurface& surface)
{
	m_surfaces.push_back(surface);
}

unsigned int CWorldGeometry::GetSurfaceCount()
{
	return m_surfaces.size();
}

vector<CWorldGeometrySurface>::iterator CWorldGeometry::GetSurfaceItr()
{
	return m_surfaces.begin();
}

const CBoundBox& CWorldGeometry::GetBoundBox()
{
	return m_boundbox;
}

/**
Fully rebuilds the bounding box for this set of geometry from its surfaces.
*/
void CWorldGeometry::BuildBoundBox()
{
	m_boundbox.ResetBounds();

	vector<CWorldGeometrySurface>::iterator itr = m_surfaces.begin();

	while(itr != m_surfaces.end())
	{
		AddToBoundBox(*itr);
	}
}

/**
Adds the given surface to the bounding box for this set of geometry.
*/
void CWorldGeometry::AddToBoundBox(CWorldGeometrySurface& surface)
{
	int i;
	for(i = 0; i < surface.GetPolygon().GetNumVertices(); i ++)
	{
		m_boundbox.UpdateBounds(surface.GetPolygon().GetVertex(i));
	}

}

void CWorldGeometry::Draw()
{
	unsigned int i;
	unsigned int a;
	for(i = 0; i < m_surfaces.size(); i++)
	{
		CWorldGeometrySurface* surface = &(m_surfaces[i]);

		glBindTexture(GL_TEXTURE_2D, surface->GetTexture());

		glNormal3fv(surface->GetPolygon().GetNormal().GetArrayPtr());
		glBegin(GL_POLYGON);
			for(a = 0; a < surface->GetPolygon().GetNumVertices(); a++)
			{
				glTexCoord2fv(surface->GetPolygon().GetTexCoord(i));
				glVertex3fv(surface->GetPolygon().GetVertex(i).GetArrayPtr());
			}
		glEnd();
	}
	
}

bool CWorldGeometry::ShouldDraw()
{
	return true;
}


