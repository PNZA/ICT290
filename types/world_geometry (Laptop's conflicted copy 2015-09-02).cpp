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
void CWorldGeometrySurface::init()
{
	static unsigned int id = 0;
	id++;
	m_geometry_surface_id = id;
}

CWorldGeometrySurface::CWorldGeometrySurface()
{
	m_texture_id = -1;
	init();
}

CWorldGeometrySurface::CWorldGeometrySurface(CPolygon& polygon, unsigned int texture)
{
	m_polygon = polygon;
	m_texture_id = texture;
	init();
}

void CWorldGeometrySurface::SetPolygon(CPolygon& polygon)
{
	m_polygon = polygon;
}

CPolygon& CWorldGeometrySurface::GetPolygon()
{
	return m_polygon;
}

int CWorldGeometrySurface::GetID()
{
	return m_geometry_surface_id;
}

int CWorldGeometrySurface::GetTexture()
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
	static unsigned int id = 0;
	id++;
	m_geometry_id = id;
}

CWorldGeometry::CWorldGeometry(vector<CWorldGeometrySurface>& surfaces, unsigned int group)
{
	m_surfaces = surfaces;
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

/**
Draws the piece of world geometry
*/
void CWorldGeometry::Draw()
{
	glCallList(m_geometry_id);
	/*
	unsigned int i;
	unsigned int a;
	for(i = 0; i < m_surfaces.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_surfaces[i].GetTexture());

		glNormal3fv(m_surfaces[i].GetPolygon().GetNormal().GetArrayPtr());
		glBegin(GL_POLYGON);
			for(a = 0; a < m_surfaces[i].GetPolygon().GetNumVertices(); a++)
			{
				glTexCoord2f(m_surfaces[i].GetPolygon().GetTexCoord(i).u,	//u axis
					m_surfaces[i].GetPolygon().GetTexCoord(i).v);			//v axis

				glVertex3fv(m_surfaces[i].GetPolygon().GetVertex(i).GetArrayPtr());
			}
		glEnd();
	}*/
	
}

void CWorldGeometry::Compile()
{
	NewList(m_geometry_id, COMPILE);
		unsigned int i;
		unsigned int a;
		for(i = 0; i < m_surfaces.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_surfaces[i].GetTexture());

			glNormal3fv(m_surfaces[i].GetPolygon().GetNormal().GetArrayPtr());
			glBegin(GL_POLYGON);
				for(a = 0; a < m_surfaces[i].GetPolygon().GetNumVertices(); a++)
				{
					glTexCoord2f(m_surfaces[i].GetPolygon().GetTexCoord(i).u,	//u axis
						m_surfaces[i].GetPolygon().GetTexCoord(i).v);			//v axis

					glVertex3fv(m_surfaces[i].GetPolygon().GetVertex(i).GetArrayPtr());
				}
			glEnd();
		}
	EndList();
}

bool CWorldGeometry::ShouldDraw()
{
	return true;
}

//-----------------------------------------------------------------------------
//GeometryManager
//-----------------------------------------------------------------------------
int GeometryManager::AreaCreate()
{
	vector<CWorldGeometry> new_area;
	m_areas.push_back(new_area);

	return m_areas.size() - 1;
}

int GeometryManager::AreaCount()
{
	return m_areas.size();
}

int GeometryManager::ActiveArea()
{
	return m_active_area;
}

int GeometryManager::GeometryInAreaCount(int area_id)
{
	assert(area_id < 0 || area_id > m_areas.size() - 1);

	return m_areas[area_id].size() - 1;
}

int GeometryManager::ActiveAreaCount()
{
	assert(m_active_area < 0);

	return m_areas[m_active_area].size() - 1;
};

void GeometryManager::SwapActiveArea(int area_id)
{
	assert(area_id < 0 || area_id > m_areas.size() - 1);

	m_active_area = area_id;
}

CWorldGeometry& GeometryManager::GetGeometry(int i)
{
	assert(i < 0 || i > m_areas[m_active_area].size() - 1);

	return m_areas[m_active_area][i];
}

void GeometryManager::AddGeometry(CWorldGeometry& geometry)
{
	assert(m_active_area < 0);

	m_areas[m_active_area].push_back(geometry);
}

//-----------------------------------------------------------------------------
//GeometryBuilder
//-----------------------------------------------------------------------------
GeometryBuilder::GeometryBuilder()
{
	m_building_obj = false;
	m_building_surf = false;
}

/**
Creates a new area and sets the active area to it.
New geometry added will appear in this new area.
*/
void GeometryBuilder::NewArea()
{
	GeometryManager& geo_manager = GeometryManager::GetInstance();
	geo_manager.SwapActiveArea(geo_manager.AreaCreate());
}

/**
Starts a new object. If an object was already started, do nothing.
*/
void GeometryBuilder::StartObject()
{
	if(!m_building_obj)
	{
		m_building_obj = true;
		m_curr_obj = CWorldGeometry();
		m_curr_surf = CWorldGeometrySurface();
	}
}

/**
Starts a new surface. If one was already started, do nothing.
*/
void GeometryBuilder::StartSurface()
{
	if(!m_building_surf)
	{
		m_building_obj = true;
		m_curr_surf = CWorldGeometrySurface();
	}
}

/**
If we're building a surface, set the current surface's texture to this.
*/
void GeometryBuilder::Texture(int tex_id)
{
	if(m_building_surf && m_building_obj)
	{
		m_curr_surf.SetTexture(tex_id);
	}
}

/**
Adds a texture coordinate to the current surface, defined by two floats.
*/
void GeometryBuilder::TextureCoordinate(float u, float v)
{
	if(m_building_surf && m_building_obj)
	{
		tex_coord_t coord;

		coord.u = u;
		coord.v = v;

		m_curr_surf.GetPolygon().AddTexCoord(coord);
	}
}

/**
Adds a texture coordinate to the current surface, defined by tex_coord_t
*/
void GeometryBuilder::TextureCoordinate(tex_coord_t& coord)
{
	if(m_building_surf && m_building_obj)
	{
		m_curr_surf.GetPolygon().AddTexCoord(coord);
	}
}

/**
Adds the given vertex to the current surface. Defined by x, y ,z.
*/
void GeometryBuilder::Vertex(float x, float y, float z)
{
	if(m_building_surf && m_building_obj)
	{
		CVector vert = CVector(x, y, z);
		m_curr_surf.GetPolygon().AddVertex(vert);
	}
}

/**
Adds the given vertex to the current surface. Defined by a float pointer
*/
void GeometryBuilder::Vertex(float* vert)
{
	if(m_building_surf && m_building_obj)
	{
		CVector vert = CVector(vert);
		m_curr_surf.GetPolygon().AddVertex(vert);
	}
}

/**
Adds the given vertex to the current surface. Defined by a CVector object
*/
void GeometryBuilder::Vertex(CVector& vert)
{
	if(m_building_surf && m_building_obj)
	{
		m_curr_surf.GetPolygon().AddVertex(vert);
	}
}

/**
Adds the current surface to the current world geometry object.
Resets the surface for the next entry.
*/
void GeometryBuilder::EndSurface()
{
	if(m_building_surf && m_building_obj)
	{
		m_building_surf = false;
		m_curr_obj.AddSurface(m_curr_surf);
		m_curr_surf = CWorldGeometrySurface();
	}
}

/**
Adds the current object to the game world geometry.
Resets the current object.
*/
void GeometryBuilder::EndObject()
{
	if(m_building_obj)
	{
		//Get the geometry manager.
		GeometryManager& geo_manager = GeometryManager::GetInstance();

		m_building_obj = false;
		geo_manager.AddGeometry(m_curr_obj);
		m_curr_obj = CWorldGeometry();
	}
}

/**
Creates a vertical wall, starting at start and going to end.
Start should be the lower
TODO - texture scale
*/
void GeometryBuilder::CreateVertical(const CVector& start, const CVector& end, float height, bool flip, float texture_u_scale, float texture_v_scale, int texture)
{
	if(m_building_obj)
	{
		if(flip == GEO_V_RIGHT)
		{
			//Right hand wall, normal pointing to the left
			StartSurface();
				Texture(texture);

				TextureCoordinate(1, 1); //bottom right
				Vertex(start.x, start.y, start.z);

				TextureCoordinate(1, 0); //top right
				Vertex(start.x, start.y + height, start.z);

				TextureCoordinate(0, 0); //top left
				Vertex(end.x, end.y + height, end.z);

				TextureCoordinate(0, 1); //bottom left
				Vertex(end.x, end.y, end.z);
			EndSurface();
		}
		else //flip == GEO_V_LEFT
		{
			//left hand wall, normal pointing to the right
			StartSurface();
				Texture(texture);

				TextureCoordinate(0, 0); //bottom left
				Vertex(start.x, start.y, start.z);

				TextureCoordinate(1, 0); //bottom right
				Vertex(end.x, end.y, end.z);

				TextureCoordinate(1, 1); //top right
				Vertex(end.x, end.y + height, end.z);

				TextureCoordinate(0, 1); //top left
				Vertex(start.x, start.y + height, start.z);
			EndSurface();
		}
	}
}

void GeometryBuilder::CreateHoriontal()