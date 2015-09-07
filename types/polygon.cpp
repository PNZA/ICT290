//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Functions for CPolygon class
//
//=============================================================================

#include "polygon.h"
#include <assert.h>
#include "../math/physics.h"

CPolygon::CPolygon()
{
	//not computed
	m_cached_midpoint = CVector(FLT_EPSILON,FLT_EPSILON,FLT_EPSILON); 
	m_normal = CVector(FLT_EPSILON,FLT_EPSILON,FLT_EPSILON);
}

CPolygon::CPolygon(vector<CVector>& verts, vector<tex_coord_t>& tex_coords)
{
	assert(verts.size() == tex_coords.size());

	//Looks a little messy, but it's so we only use one iterator instead of two.
	vector<CVector>::iterator vert_itr;
	unsigned int i = 0;

	for(vert_itr = verts.begin(); vert_itr!= verts.end(); vert_itr++)
	{
		m_verts.push_back(*vert_itr);
		m_tex_coords.push_back((tex_coords)[i]);
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

//-----------------------------------------------------------------------------
//Iterators
//-----------------------------------------------------------------------------
vector<CVector>::iterator CPolygon::GetVerticesBegin()
{
	return m_verts.begin();
}

vector<CVector>::const_iterator CPolygon::GetVerticesBegin() const
{
	return m_verts.begin();
}

vector<CVector>::iterator CPolygon::GetVerticesEnd()
{
	return m_verts.end();
}

vector<CVector>::const_iterator CPolygon::GetVerticesEnd() const
{
	return m_verts.end();
}

vector<tex_coord_t>::iterator CPolygon::GetTexCoordsBegin()
{
	return m_tex_coords.begin();
}

vector<tex_coord_t>::const_iterator CPolygon::GetTexCoordsBegin() const
{
	return m_tex_coords.begin();
}

vector<tex_coord_t>::iterator CPolygon::GetTexCoordsEnd()
{
	return m_tex_coords.end();
}

vector<tex_coord_t>::const_iterator CPolygon::GetTexCoordsEnd() const
{
	return m_tex_coords.end();
}

//-----------------------------------------------------------------------------
//Add functions
//-----------------------------------------------------------------------------
void CPolygon::AddVertex(CVector& vertex)
{
	m_verts.push_back(vertex);
}

void CPolygon::AddTexCoord(tex_coord_t& coord)
{
	m_tex_coords.push_back(coord);
}

//-----------------------------------------------------------------------------
//Manual get and cached get functions
//-----------------------------------------------------------------------------
const CVector& CPolygon::GetVertex(unsigned int i) const
{
	assert(i >= 0 && i < m_verts.size());
	return m_verts[i];
}

const tex_coord_t& CPolygon::GetTexCoord(unsigned int i) const
{
	assert(i >= 0 && i < m_tex_coords.size());
	return m_tex_coords[i];
}

const CVector& CPolygon::GetNormal()
{
	ComputeNormal();
	return m_normal;
}

const CVector& CPolygon::GetCachedNormal() const
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

const CVector& CPolygon::GetCachedMidpoint() const
{
	return m_cached_midpoint;
}

unsigned int CPolygon::GetNumVertices() const
{
	return m_verts.size();
}

unsigned int CPolygon::GetNumTexCoords() const
{
	return m_tex_coords.size();
}

CPlane CPolygon::GetPlane() const
{
	return CPlane(GetVertex(0), m_normal);
}

//-----------------------------------------------------------------------------
//Calculation functions
//-----------------------------------------------------------------------------

bool CPolygon::IsValid() const
{
	if (m_verts.size() < 2 && m_tex_coords.size() < 2)
		return false;

	return true;
}

void CPolygon::ComputeNormal()
{
	if(!IsValid())
		return;

	(m_verts[1] - m_verts[0]).CrossProduct((m_verts[2] - m_verts[0]), m_normal);
	m_normal.Normalise();
}

/**
Scales this polygon to the given scale vector.
*/
void CPolygon::Scale(const CVector& scale)
{
	int i;
	for(i = 0; i < m_verts.size(); i++)
	{
		m_verts[i] *= scale;
	}

	m_normal *= scale;
	m_cached_midpoint *= scale;
}
