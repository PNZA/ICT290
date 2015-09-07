//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements functions for the rayplane class. Can represent either a ray in 3D space,
//			or a plane in 3D space.
//
//=============================================================================

#include "rayplane.h"


CRayPlane::CRayPlane()
{
	m_nCalculated = false;
	m_lCalculated = false;
	m_dCalculated = false;
}

CRayPlane::CRayPlane(const CVector& origin, const CVector& vector)
{
	m_origin = origin;
	m_vector = vector;
	m_nCalculated = false;
	m_lCalculated = false;
	m_dCalculated = false;
	m_vector.GetLength();
}

CRayPlane::~CRayPlane()
{
	return;
};

const float CRayPlane::Distance(const CVector& point) const
{
	float t;
	Intersect(CRayPlane(point, -GetNormal()), t);
	return t;
}

const bool CRayPlane::Intersect(CRayPlane& r, float &t) const
{
	t = 0;
	float denom = GetNormal().DotProduct(r.GetNormal());

	if(denom == 0) return false;

	float numer = GetNormal().DotProduct(r.GetOrigin());

	t = -((numer + D()) / denom);

	return true;
}

const CVector CRayPlane::Closest(const CVector& point) const
{
	float t = Distance(point);
	return point - GetNormal() * t;
}

const float CRayPlane::Length() const
{
	if(!m_lCalculated)
	{
		m_length = m_vector.GetCachedLength();
		m_lCalculated = true;
	}

	return m_length;
}

void CRayPlane::SetLength(float len)
{
	if(m_nCalculated)
		m_vector = m_normal * len;
	else
	{
		m_vector.Normalise();
		m_vector *= len;
	}

	m_length = len;
	m_lCalculated = true;
}

float CRayPlane::D() const
{
	if(!m_dCalculated)
	{
		d = -(GetOrigin().DotProduct(GetNormal())); //normal might not be calculated
		m_dCalculated = true;
	}

	return d;
}

const CVector& CRayPlane::GetNormal() const
{
	if(!m_nCalculated)
	{
		m_normal = m_vector;
		m_normal.Normalise();
		m_nCalculated = true;
	}

	return m_normal;
}

CVector CRayPlane::End() const
{
	return m_origin + m_vector;
}

CVector CRayPlane::End(float len) const
{
	return (m_origin + GetNormal()) * len;
}

const CVector& CRayPlane::GetOrigin() const
{
	return m_origin;
}

const CVector& CRayPlane::GetVector() const
{
	return m_vector;
}

void CRayPlane::SetOrigin(const CVector& origin)
{
	m_origin = origin;
	m_dCalculated = false;
}

void CRayPlane::SetVector(const CVector& vector)
{
	m_vector = vector;
	m_lCalculated = false;
	m_dCalculated = false;
	m_vector.GetLength();
}