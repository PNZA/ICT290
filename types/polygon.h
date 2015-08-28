//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Represents a polygon in 3D space. Convex, containing at least 3 points.
//
//=============================================================================

#ifndef CPOLYGON_H
#define CPOLYGON_H

#include "vector.h"
#include <vector>

using namespace std;

typedef float tex_coord_t[2];

class CPolygon
{
private:
	vector<CVector> m_verts;
	vector<tex_coord_t> m_tex_coords;

	CVector m_normal;
	CVector m_cached_midpoint;

	void ComputeNormal();

public:
	CPolygon();
	CPolygon(vector<CVector> *verts, vector<tex_coord_t> *tex_coords);
	~CPolygon();

	vector<CVector>::iterator GetVerticesItr();
	vector<tex_coord_t>::iterator GetTexCoordsItr();

	const CVector& GetVertex(unsigned int i);
	const tex_coord_t& GetTexCoord(unsigned int i);

	const CVector& GetNormal();
	const CVector& GetMidpoint();
	const CVector& GetCachedMidpoint();
	unsigned int GetNumVertices();
	unsigned int GetNumTexCoords();
	bool IsValid();

	void AddVertex(CVector& vertex);
	void AddTexCoord(tex_coord_t& coord);

	CPolygon& operator=(const CPolygon& otherPoly);
};

inline CPolygon& CPolygon::operator=(const CPolygon& otherPoly)
{
	m_verts = otherPoly.m_verts;
	m_tex_coords = otherPoly.m_tex_coords;
}

#endif