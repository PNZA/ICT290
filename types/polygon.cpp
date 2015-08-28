//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Functions for CPolygon class
//
//=============================================================================

#include "polygon.h"
#include <assert.h>

CPolygon::CPolygon()
{
	//not computed
	m_cached_midpoint = NULL; 
	m_normal = NULL;
}

CPolygon::CPolygon(vector<CVector> *verts, vector<tex_coord_t> *tex_coords)
{
	assert(verts->size() == tex_coords->size());

	//Looks a little messy, but it's so we only use one iterator instead of two.
	vector<CVector>::iterator vert_itr;
	unsigned int i = 0;

	for(vert_itr = verts->begin(); vert_itr!= verts->end(); vert_itr++)
	{
		m_verts.push_back(*vert_itr);
		m_verts.push_back((*tex_coords)[i]);
		i++;
	}

	//make sure it was all copied over correctly
	assert(m_verts.size() == m_tex_coords.size());

	ComputeNormal();
	m_cached_midpoint = GetMidpoint();
}

CPolygon::~CPolygon()
{
	m_verts.clear();
	m_tex_coords.clear();
}

vector<CVector>::iterator CPolygon::GetVerticesItr()
{
	return m_verts.begin();
}

vector<tex_coord_t>::iterator CPolygon::GetTexCoordsItr()
{
	return m_tex_coords.begin();
}

const CVector& CPolygon::GetVertex(unsigned int i)
{
	assert(i >= 0 && i <= m_verts.size());
	return m_verts[i];
}

const tex_coord_t& CPolygon::GetTexCoord(unsigned int i)
{
	assert(i >= 0 && i <= m_tex_coords.size());
	return m_tex_coords[i];
}

const CVector& CPolygon::GetNormal()
{
	return m_normal;
}

const CVector& CPolygon::GetMidpoint()
{
	unsigned int i;

	m_cached_midpoint.x = 0;
	m_cached_midpoint.y = 0;
	m_cached_midpoint.z = 0;

	//add up all vertices
	for(i = 0; i < m_verts.size(); i++)
	{
		m_cached_midpoint += m_verts[i];
	}

	//divide by number of vertices added
	m_cached_midpoint /= (i+1);

	return m_cached_midpoint;
}

const CVector& CPolygon::GetCachedMidpoint()
{
	return m_cached_midpoint;
}

unsigned int CPolygon::GetNumVertices()
{
	return m_verts.size();
}

unsigned int CPolygon::GetNumTexCoords()
{
	return m_tex_coords.size();
}

bool CPolygon::IsValid()
{
	if(m_normal != NULL && m_verts.size() > 2 && m_tex_coords.size() > 2)
		return true;

	return false;
}

void CPolygon::AddVertex(CVector& vertex)
{
	m_verts.push_back(vertex);
}

void CPolygon::AddTexCoord(tex_coord_t& coord)
{
	m_tex_coords.push_back(coord);
}