//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements a 3D ray plane class. Can represent either a ray in 3D space,
//			or a plane in 3D space.
//
//=============================================================================
#ifndef RAYPLANE_H
#define RAYPLANE_H

#include "vector.h"

class CRayPlane
{
private:
	CVector m_origin;
	CVector m_vector;
	mutable CVector m_normal;

	mutable bool m_nCalculated;
	mutable bool m_lCalculated;
	mutable bool m_dCalculated;

	mutable float m_length;
	mutable float d; //D of plane equation

public:
	CRayPlane();
	CRayPlane(const CVector& origin, const CVector& vector);

	~CRayPlane();

	const float Distance(const CVector& point) const;
	const bool Intersect(CRayPlane& r, float &t) const;
	const CVector Closest(const CVector& point) const;
	CVector End() const;
	CVector End(float len) const;

	const CVector& GetOrigin() const;
	const CVector& GetVector() const;
	const CVector& GetNormal() const;
	const float Length() const;
	float D() const;

	void SetOrigin(const CVector& origin);
	void SetVector(const CVector& vector);
	void SetLength(float len);

	inline CRayPlane& operator=(const CRayPlane &r)
	{
		m_origin = r.m_origin;
		m_vector = r.m_vector;
		m_normal = r.m_normal;
		m_length = r.m_length;

		m_lCalculated = r.m_lCalculated;
		m_lCalculated = r.m_lCalculated;
		m_dCalculated = r.m_dCalculated;

		d = r.d;

		return *this;
	}

};

//ehehehehehe
typedef CRayPlane CPlane;
typedef CRayPlane CRay;

#endif