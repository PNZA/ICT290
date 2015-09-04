//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Functions for the bounding box classes
//
//=============================================================================

#include "bound_box.h"

/**
Initialises this bounding box to all 0's.
*/
CBoundBox::CBoundBox()
{
	m_min.x = m_min.y = m_min.z = 0.0;
	m_max.x = m_max.y = m_max.z = 0.0;
}

/**
No destruction needed
*/
CBoundBox::~CBoundBox()
{
	return;
}

void CBoundBox::ResetBounds()
{
	m_min.x = m_min.y = m_min.z = 0.0;
	m_max.x = m_max.y = m_max.z = 0.0;
}

/**
Sets the bounds of this box outright to the given values.
*/
void CBoundBox::SetBounds(const CVector& min, const CVector& max)
{
	m_min.x = min.x;
	m_min.y = min.y;
	m_min.z = min.z;

	m_max.x = max.x;
	m_max.x = max.y;
	m_max.x = max.z;
}

/**
Make this bounding box include the bounds given by min and max.
*/
void CBoundBox::UpdateBounds(const CVector& min, const CVector& max)
{
	if(min.x < m_min.x)
		m_min.x = min.x;
	if(min.y < m_min.y)
		m_min.y = min.y;
	if(min.z < m_min.z)
		m_min.z = min.z;


	if(max.x > m_max.x)
		m_max.x = max.x;
	if(max.y > m_max.y)
		m_max.y = max.y;
	if(max.z > m_max.z)
		m_max.z = max.z;
}

/**
Updates the bounds of this box to include the given point
*/
void CBoundBox::UpdateBounds(const CVector& point)
{

	//Needed because the box might not have a min yet.
	if(m_min.x == 0 || m_min.y == 0 || m_min.y == 0)
	{
		m_min = point;
	}
	else
	{
		if(point.x < m_min.x)
			m_min.x = point.x;
		if(point.y < m_min.y)
			m_min.y = point.y;
		if(point.z < m_min.z)
			m_min.z = point.z;
	}



	if(point.x > m_max.x)
		m_max.x = point.x;
	if(point.y > m_max.y)
		m_max.y = point.y;
	if(point.z > m_max.z)
		m_max.z = point.z;
}

/**
Updates the bounds of this box to include the other box
*/
void CBoundBox::UpdateBounds(const CBoundBox& otherBox)
{
	UpdateBounds(otherBox.m_min, otherBox.m_max);
}

/**
Gets the centre of this box
*/
void CBoundBox::GetBoundsCentre(CVector& centre) const
{
	centre.x = (m_min.x - m_max.x) * 0.5;
	centre.y = (m_min.y - m_max.y) * 0.5;
	centre.z = (m_min.z - m_max.z) * 0.5;
}

/**
Gets a vector from this box's min to this box's max
*/
void CBoundBox::GetBoundsSize(CVector& size) const
{
	size.x = m_max.x - m_min.x;
	size.y = m_max.y - m_min.y;
	size.z = m_max.z - m_min.z;
}

/**
Returns true if the given point is inside the box (will not return true if on edge!)
*/
bool CBoundBox::ContainsPoint(const CVector& point) const
{
	if(point.x < m_min.x || point.x > m_max.x)
		return false;

	if(point.y < m_min.y || point.y > m_max.y)
		return false;

	if(point.z < m_min.z || point.z > m_max.z)
		return false;

	return true;
}

/**
Translates the box in the given direction.
*/
void CBoundBox::Translate(const CVector& direction)
{
	m_min += direction;
	m_max += direction;
}

/**
Returns true if the given box is intersecting this box
*/
bool CBoundBox::IsIntersectingBox(const CBoundBox& otherBox) const
{
	if(m_min.x >= otherBox.m_max.x || m_max.x <= otherBox.m_min.x)
		return false;

	if(m_min.y >= otherBox.m_max.y || m_max.y <= otherBox.m_min.y)
		return false;

	if(m_min.z >= otherBox.m_max.z || m_max.z <= otherBox.m_min.z)
		return false;

	return true;
}

/**
Returns true if the box composed of the given vectors intersects this box
*/
bool CBoundBox::IsIntersectingBox(const CVector& min, const CVector& max) const
{
	if(m_min.x >= max.x || m_max.x <= min.x)
		return false;

	if(m_min.y >= max.y || m_max.y <= min.y)
		return false;

	if(m_min.z >= max.z || m_max.z <= min.z)
		return false;

	return true;
}

/**
Returns true if this box is inside the other box.
*/
bool CBoundBox::IsInsideBox(const CBoundBox& otherBox) const
{
	if(m_min.x < otherBox.m_min.x || m_max.x > otherBox.m_max.x)
		return false;

	if(m_min.y < otherBox.m_min.y || m_max.y > otherBox.m_max.y)
		return false;

	if(m_min.z < otherBox.m_min.z || m_max.z > otherBox.m_max.z)
		return false;

	return true;
}

/**
Returns true if this box is inside the box composed of the given vectors.
*/
bool CBoundBox::IsInsideBox(const CVector& min, CVector& max) const
{
	if(m_min.x < min.x || m_max.x > max.x)
		return false;

	if(m_min.y < min.y || m_max.y > max.y)
		return false;

	if(m_min.z < min.z || m_max.z > max.z)
		return false;

	return true;
}

/**
Returns true if this box is valid (min < max)
*/
bool CBoundBox::IsValid() const
{
	if(m_min.x > m_max.x)
		return false;

	if(m_min.y > m_max.z)
		return false;

	if(m_min.z > m_max.z)
		return false;

	return true;
}