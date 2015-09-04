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

typedef struct
{
	float u;
	float v;

}  tex_coord_t;

class CPolygon
{
protected:
	vector<CVector> m_verts;
	vector<tex_coord_t> m_tex_coords;

	CVector m_normal;
	CVector m_cached_midpoint;

public:
	CPolygon();
	CPolygon(vector<CVector>& verts, vector<tex_coord_t>& tex_coords);
	~CPolygon();

	vector<CVector>::iterator GetVerticesBegin();
	vector<CVector>::iterator GetVerticesEnd();
	vector<tex_coord_t>::iterator GetTexCoordsBegin();
	vector<tex_coord_t>::iterator GetTexCoordsEnd();

	const CVector& GetVertex(unsigned int i) const;
	const tex_coord_t& GetTexCoord(unsigned int i) const;

	const CVector& GetNormal();
	const CVector& GetMidpoint();
	const CVector& GetCachedMidpoint() const;
	const CVector& GetCachedNormal() const;
	unsigned int GetNumVertices() const;
	unsigned int GetNumTexCoords() const;
	bool IsValid() const;

	void AddVertex(CVector& vertex);
	void AddTexCoord(tex_coord_t& coord);
	void ComputeNormal();

	CPolygon& operator=(const CPolygon& otherPoly);
};

inline CPolygon& CPolygon::operator=(const CPolygon& otherPoly)
{
	m_verts = otherPoly.m_verts;
	m_tex_coords = otherPoly.m_tex_coords;
	return *this;
}

inline bool PolygonContainsPoint()
{

}

#endif