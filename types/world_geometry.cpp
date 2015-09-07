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
	static unsigned int surf_id = 0;
	surf_id++;
	m_geometry_surface_id = surf_id;
}

CWorldGeometrySurface::CWorldGeometrySurface()
{
	m_texture_id = -1;
	init();
}

CWorldGeometrySurface::CWorldGeometrySurface(CPolygon& polygon, unsigned int texture)
{
	(CPolygon)*this = polygon;
	m_texture_id = texture;
	init();
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
	static unsigned int geo_id = 0;
	geo_id++;
	m_geometry_id = geo_id;
}

CWorldGeometry::CWorldGeometry(vector<CWorldGeometrySurface>& surfaces)
{
	m_surfaces = surfaces;
	BuildBoundBox();
}

void CWorldGeometry::AddSurface(CWorldGeometrySurface& surface)
{
	AddToBoundBox(surface);
	m_surfaces.push_back(surface);
}

unsigned int CWorldGeometry::GetSurfaceCount()
{
	return m_surfaces.size();
}

const CWorldGeometrySurface& CWorldGeometry::GetSurface(int i)
{
	assert(i >= 0 || i < m_surfaces.size());
	return m_surfaces[i];
}

vector<CWorldGeometrySurface>::iterator CWorldGeometry::GetSurfaceStart()
{
	return m_surfaces.begin();
}

vector<CWorldGeometrySurface>::iterator CWorldGeometry::GetSurfaceEnd()
{
	return m_surfaces.end();
}

const CBoundBox& CWorldGeometry::GetBoundBox() const
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
		itr++;
	}

	CVector threshold = CVector(10, 10, 10);
	m_boundbox.m_min -= threshold;
	m_boundbox.m_max += threshold;
}

/**
Adds the given surface to the bounding box for this set of geometry.
*/
void CWorldGeometry::AddToBoundBox(CWorldGeometrySurface& surface)
{
	int i;
	for(i = 0; i < surface.GetNumVertices(); i ++)
	{
		m_boundbox.UpdateBounds(surface.GetVertex(i));
	}

}

/**
Draws the piece of world geometry
*/
void CWorldGeometry::Draw()
{
	glCallList(1000 + m_geometry_id);
	
	glPushMatrix();
	glLineWidth(2);
	glColor3f(255,0,0);
	glBegin(GL_LINE_LOOP);
		glVertex3fv(m_boundbox.m_min.GetArrayPtr());
		glVertex3fv(m_boundbox.m_max.GetArrayPtr());
	glEnd();
	glPopMatrix();
	
	/*unsigned int i, a;
	for(i = 0; i < m_surfaces.size(); i++)
	{
		printf("\nCWordGeometry::Compile() Surface\n");
		glBindTexture(GL_TEXTURE_2D, m_surfaces[i].GetTexture());
		printf("CWorldGeometry::Compile() Texture: %d\n", m_surfaces[i].GetTexture());
		glNormal3fv(m_surfaces[i].GetNormal().GetArrayPtr());
		printf("CWorldGeometry::Compile() Normal: %f, %f, %f\n", 
			m_surfaces[i].GetNormal().GetArrayPtr()[0], 
			m_surfaces[i].GetNormal().GetArrayPtr()[1],
			m_surfaces[i].GetNormal().GetArrayPtr()[2]);
		glBegin(GL_POLYGON);
			for(a = 0; a < m_surfaces[i].GetNumVertices(); a++)
			{
				glTexCoord2f(m_surfaces[i].GetTexCoord(i).u,	//u axis
					m_surfaces[i].GetTexCoord(i).v);			//v axis

				printf("CWorldGeometry::Compile() TexCoord: %f, %f\n", m_surfaces[i].GetTexCoord(i).u,
					m_surfaces[i].GetTexCoord(i).u);

				glVertex3fv(m_surfaces[i].GetVertex(i).GetArrayPtr());
				printf("CWorldGeometry::Compile() Vert: %f, %f, %f\n", m_surfaces[i].GetVertex(i).GetArrayPtr()[0],
					m_surfaces[i].GetVertex(i).GetArrayPtr()[1],
					m_surfaces[i].GetVertex(i).GetArrayPtr()[2]);
			}
		glEnd();
		}*/
}

void CWorldGeometry::Compile()
{
	glNewList(1000 + m_geometry_id, GL_COMPILE); //need to add this to make sure we're not touching shays
		unsigned int i, a;
		for(i = 0; i < m_surfaces.size(); i++)
		{
			//printf("\nCWordGeometry::Compile() Surface %d\n", i);
			glBindTexture(GL_TEXTURE_2D, m_surfaces[i].GetTexture());
			//printf("CWorldGeometry::Compile() Texture: %d\n", m_surfaces[i].GetTexture());
			glNormal3fv(m_surfaces[i].GetNormal().GetArrayPtr());
			//printf("CWorldGeometry::Compile() Normal: %f, %f, %f\n", 
			//	m_surfaces[i].GetNormal().GetArrayPtr()[0], 
			//	m_surfaces[i].GetNormal().GetArrayPtr()[1],
			//	m_surfaces[i].GetNormal().GetArrayPtr()[2]);
			glBegin(GL_POLYGON);
				for(a = 0; a < m_surfaces[i].GetNumVertices(); a++)
				{
					glTexCoord2f(m_surfaces[i].GetTexCoord(a).u,	//u axis
						m_surfaces[i].GetTexCoord(a).v);			//v axis

					//printf("CWorldGeometry::Compile() TexCoord: %f, %f\n", m_surfaces[i].GetTexCoord(a).u,
					//	m_surfaces[i].GetTexCoord(a).u);

					glVertex3fv(m_surfaces[i].GetVertex(a).GetArrayPtr());
					//printf("CWorldGeometry::Compile() Vert: %f, %f, %f\n", m_surfaces[i].GetVertex(a).GetArrayPtr()[0],
					//	m_surfaces[i].GetVertex(a).GetArrayPtr()[1],
					//	m_surfaces[i].GetVertex(a).GetArrayPtr()[2]);
				}
			glEnd();
		}
	glEndList();
	printf("Making bound box...\n");
	BuildBoundBox();
	printf("CWorldGeometry: Compiled geometry %d\n", 1000 + m_geometry_id);
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

/**
If the active areas array contains this area, return where. else -1
*/
int GeometryManager::ActiveContainsArea(int area_id)
{
	int i;
	for(i = 0; i < m_active_areas.size(); i++)
	{
		if(area_id == m_active_areas[i])
		{
			//printf("GeometryManager: Area %d is active at %d\n", area_id, i);
			return i;
		}
			
	}
	//printf("GeometryManager: Area %d is not active\n", area_id);

	return -1;
}

int GeometryManager::AreaCount()
{
	return m_areas.size();
}

/**
Add an area to the active areas list
*/
void GeometryManager::EnableArea(int area_id)
{
	if(ActiveContainsArea(area_id) >= 0)
	{
		//printf("GeometryManager: Area %d already active\n", area_id);
		return;
	}

	//printf("GeometryManager: Enabling area %d\n", area_id);

	m_active_areas.push_back(area_id);
}

/**
Erase an area from the active areas list
*/
void GeometryManager::DisableArea(int area_id)
{
	int loc;
	if(loc = ActiveContainsArea(area_id))
	{
		m_active_areas.erase(m_active_areas.begin() + loc);
	}
}

bool GeometryManager::IsAreaActive(int area_id)
{
	if(ActiveContainsArea(area_id))
	{
		return true;
	}

	return false;
}

int GeometryManager::GeometryInAreaCount(int area_id)
{
	assert(area_id >= 0 || area_id < m_areas.size());

	return m_areas[area_id].size();
}

void GeometryManager::AddGeometry(CWorldGeometry& geometry, int area_id)
{
	assert(area_id >= 0 || area_id < m_areas.size());

	m_areas[area_id].push_back(geometry);
}

CWorldGeometry& GeometryManager::GetGeometry(int area_id, int geo_id)
{
	assert(area_id >= 0 || area_id < m_areas.size() - 1);
	assert(geo_id >= 0 || geo_id < m_areas[area_id].size() - 1);

	return m_areas[area_id][geo_id];
}

void GeometryManager::CompileAll()
{
	int i, j;
	for(i = 0; i < m_areas.size(); i++)
	{
		for(j = 0; j < m_areas[i].size(); j++)
		{
			m_areas[i][j].Compile();
		}
	}
}

//-----------------------------------------------------------------------------
//GeometryBuilder
//-----------------------------------------------------------------------------
GeometryBuilder::GeometryBuilder()
{
	m_curr_area = -1;
	m_building_obj = false;
	m_building_surf = false;
}

/**
Creates a new area and sets the active area to it.
New geometry added will appear in this new area.
*/
int GeometryBuilder::NewArea()
{	
	GeometryManager& geo_manager = GeometryManager::GetInstance();
	m_curr_area = geo_manager.AreaCreate();
	//printf("GeometryBuilder: Making new area. id = %d\n", m_curr_area);
	return m_curr_area;
}

/**
Starts a new object. If an object was already started, do nothing.
*/
void GeometryBuilder::StartObject()
{
	if(!m_building_obj)
	{
		//printf("GeometryBuilder: Starting new object\n");
		m_building_obj = true;
	}
}

/**
Starts a new surface. If one was already started, do nothing.
*/
void GeometryBuilder::StartSurface()
{
	if(m_building_obj && !m_building_surf)
	{
		//printf("GeometryBuilder: Starting new surface\n");
		m_building_surf = true;
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
		//printf("GeometryBuilder: Adding texture %d\n", tex_id);
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
		//printf("GeometryBuilder: Adding texture coordinate %f %f\n", u, v);
		tex_coord_t coord;

		coord.u = u;
		coord.v = v;

		m_curr_surf.AddTexCoord(coord);
	}
}

/**
Adds a texture coordinate to the current surface, defined by tex_coord_t
*/
void GeometryBuilder::TextureCoordinate(tex_coord_t& coord)
{
	if(m_building_surf && m_building_obj)
	{
		//printf("GeometryBuilder: Adding texture coordinate %f %f\n", coord.u, coord.v);
		m_curr_surf.AddTexCoord(coord);
	}
}

/**
Adds the given vertex to the current surface. Defined by x, y ,z.
*/
void GeometryBuilder::Vertex(float x, float y, float z)
{
	if(m_building_surf && m_building_obj)
	{
		//printf("GeometryBuilder: Adding vertex %f %f %f\n", x, y, z);
		CVector vert = CVector(x, y, z);
		Vertex(vert);
	}
}

/**
Adds the given vertex to the current surface. Defined by a float pointer
*/
void GeometryBuilder::Vertex(float* vert)
{
	if(m_building_surf && m_building_obj)
	{
		CVector vertex = CVector(vert);
		//cout << "GeometryBuilder: Adding vertex " << vertex << "\n";

		Vertex(vertex);
	}
}

/**
Adds the given vertex to the current surface. Defined by a CVector object
*/
void GeometryBuilder::Vertex(CVector& vert)
{
	if(m_building_surf && m_building_obj)
	{
		//cout << "GeometryBuilder: Adding vertex " << vert << "\n";
		m_curr_surf.AddVertex(vert);
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

		m_curr_surf.ComputeNormal();
		m_curr_surf.GetMidpoint();

		m_curr_obj.AddSurface(m_curr_surf);
		m_curr_surf = CWorldGeometrySurface();
		//printf("GeometryBuilder: Ending Surface\n");
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
		//printf("GeometryBuilder: Ending Object\n");
		//printf("GeometryBuilder: Total surfaces: %d\n", m_curr_obj.GetSurfaceCount());
		//Get the geometry manager.
		GeometryManager& geo_manager = GeometryManager::GetInstance();

		m_building_obj = false;
		geo_manager.AddGeometry(m_curr_obj, m_curr_area);
		m_curr_obj = CWorldGeometry();
	}
}

/**
Gets the correct u and v axis for rotating a texture.
*/
/*
void GeometryBuilder::TextureRotation(float& u, float& v, const STexInfo& info)
{
	float u_rot = sin(info.rotation);
	float v_rot = cos(info.rotation);

	u *= u_rot;
	v *= v_rot;

}*/


//----------------------NOTE----------------------
//The texture rotation system at the moment is hacky because we don't have time to implement
//proper calculated rotation. This will not be the case in assignment 2.

/**
Creates a vertical wall, starting at start and going to end.
Start should be the lower
*/
void GeometryBuilder::CreateVertical(const CVector& start, const CVector& end, float height, bool flip, const STexInfo& texture_info)
{
	if(m_building_obj)
	{
		if(flip == GEO_V_RIGHT)
		{
			//printf("GeometryBuilder: Adding a right wall\n");
			//Right hand wall, normal pointing to the left
			if(texture_info.rotation90) //TODO change this system ENTIRELY.
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(start.x, start.y + height, start.z);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(end.x, end.y + height, end.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(end.x, end.y, end.z);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(start.x, start.y + height, start.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(end.x, end.y + height, end.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(end.x, end.y, end.z);
				EndSurface();
			}

		}
		else //flip == GEO_V_LEFT
		{
			//printf("GeometryBuilder: Adding a left wall\n");
			//left hand wall, normal pointing to the right
			if(texture_info.rotation90)
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(end.x, end.y, end.z);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(end.x, end.y + height, end.z);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(start.x, start.y + height, start.z);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(end.x, end.y, end.z);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(end.x, end.y + height, end.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(start.x, start.y + height, start.z);
				EndSurface();
			}
		}
	}
}

void GeometryBuilder::CreateHorizontal(const CVector& start, const CVector& end, float width, bool flip, const STexInfo& texture_info)
{
	if(m_building_obj)
	{
		if(flip == GEO_H_DOWN)
		{
			//printf("GeometryBuilder: Adding a floor\n");
			if(texture_info.rotation90)
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(start.x, start.y, start.z + width);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(end.x, end.y, end.z + width);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(end.x, end.y, end.z);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(start.x, start.y, start.z + width);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(end.x, end.y, end.z + width);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(end.x, end.y, end.z);
				EndSurface();
			}
		}
		else //flip == GEO_H_UP
		{

			if(texture_info.rotation90)
			{
			//printf("GeometryBuilder: Adding a roof\n");
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(end.x, end.y, end.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(end.x, end.y, end.z + width);

					TextureCoordinate(-1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(start.x, start.y, start.z + width);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(texture_info.tex_id);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top left
					Vertex(start.x, start.y, start.z);

					TextureCoordinate(0 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom left
					Vertex(end.x, end.y, end.z);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 0 * texture_info.tex_scale_v + texture_info.tex_offset_v); //bottom right
					Vertex(end.x, end.y, end.z + width);

					TextureCoordinate(1 * texture_info.tex_scale_u + texture_info.tex_offset_u, 1 * texture_info.tex_scale_v + texture_info.tex_offset_v); //top right
					Vertex(start.x, start.y, start.z + width);
				EndSurface();
			}
		}
	}
}
// Adam's stair building code
void GeometryBuilder::CreateStairs(const CVector& start, const CVector& end, int StepCount, const STexInfo& stair_top, const STexInfo& stair_side)
{
	if(m_building_obj)
	{
		CVector total_run, total_rise, total_width;
		total_run.x = end.x - start.x;   // x,0,0
		total_rise.y = end.y - start.y;  // 0,y,0
		total_width.z = end.z - start.z; // 0,0,z
		CVector run = total_run/StepCount;
		CVector rise = total_rise/(StepCount + 1);

		//printf("GeometryBuilder: Building stairs with %d stairs\n", StepCount);
		CVector v1,v2,v3,v4;
		v1 = start;
		v4 = v1 + total_width;
		for( int i = 0; i < StepCount; i++ )
		{
			v2 = v1 + rise;
			v3 = v4 + rise;
			//draw ^ v1 -> v2 -> v3 -> v4
			if(stair_side.rotation90)
			{
				StartSurface();
					Texture(stair_side.tex_id);
					TextureCoordinate(0 * stair_side.tex_scale_u + stair_side.tex_offset_u, 1 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v1);
					TextureCoordinate(-1 * stair_side.tex_scale_u + stair_side.tex_offset_u, 1 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v2);
					TextureCoordinate(-1 * stair_side.tex_scale_u + stair_side.tex_offset_u, 0 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v3);
					TextureCoordinate(0 * stair_side.tex_scale_u + stair_side.tex_offset_u, 0 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v4);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(stair_side.tex_id);
					TextureCoordinate(1 * stair_side.tex_scale_u + stair_side.tex_offset_u, 0 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v1);
					TextureCoordinate(1 * stair_side.tex_scale_u + stair_side.tex_offset_u, 1 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v2);
					TextureCoordinate(0 * stair_side.tex_scale_u + stair_side.tex_offset_u, 1 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v3);
					TextureCoordinate(0 * stair_side.tex_scale_u + stair_side.tex_offset_u, 0 * stair_side.tex_scale_v + stair_side.tex_offset_v);
					Vertex(v4);
				EndSurface();
			}

			v1 = v2 + run;
			v4 = v3 + run;
			//draw ^ v2 -> v1 -> v4 -> v3
			if(stair_top.rotation90)
			{
				StartSurface();
					Texture(stair_top.tex_id);
					TextureCoordinate(0 * stair_top.tex_scale_u + stair_top.tex_offset_u, 1 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v2);												
					TextureCoordinate(-1 * stair_top.tex_scale_u + stair_top.tex_offset_u, 1 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v1);												
					TextureCoordinate(-1 * stair_top.tex_scale_u + stair_top.tex_offset_u, 0 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v4);												
					TextureCoordinate(0 * stair_top.tex_scale_u + stair_top.tex_offset_u, 0 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v3);
				EndSurface();
			}
			else
			{
				StartSurface();
					Texture(stair_top.tex_id);
					TextureCoordinate(1 * stair_top.tex_scale_u + stair_top.tex_offset_u, 0 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v2);												
					TextureCoordinate(1 * stair_top.tex_scale_u + stair_top.tex_offset_u, 1 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v1);												
					TextureCoordinate(0 * stair_top.tex_scale_u + stair_top.tex_offset_u, 1 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v4);												
					TextureCoordinate(0 * stair_top.tex_scale_u + stair_top.tex_offset_u, 0 * stair_top.tex_scale_v + stair_top.tex_offset_v);
					Vertex(v3);
				EndSurface();
			}
		}
	}
}

void GeometryBuilder::CreateRectangularPrism(const CVector& bottom_left, float length, float width, float height, const STexInfo& sides, const STexInfo& top)
{
	if(m_building_obj)
	{
		CreateVertical(bottom_left, CVector(bottom_left.x, bottom_left.y, bottom_left.z + length), height, GEO_V_LEFT, sides);
		CreateVertical(bottom_left, CVector(bottom_left.x + width, bottom_left.y, bottom_left.z), height, GEO_V_RIGHT, sides);
		CreateVertical(CVector(bottom_left.x, bottom_left.y, bottom_left.z + length), CVector(bottom_left.x + width, bottom_left.y, bottom_left.z + length), height, GEO_V_LEFT, sides);
		CreateVertical(CVector(bottom_left.x + width, bottom_left.y, bottom_left.z), CVector(bottom_left.x + width, bottom_left.y, bottom_left.z + length), height, GEO_V_RIGHT, sides);

		CreateHorizontal(bottom_left, CVector(bottom_left.x + width, bottom_left.y, bottom_left.z), length, GEO_H_UP, top);
		CreateHorizontal(CVector(bottom_left.x, bottom_left.y + height, bottom_left.z), CVector(bottom_left.x + width, bottom_left.y + height, bottom_left.z), length, GEO_H_DOWN, top);
	}

}
