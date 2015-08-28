//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Bounding Box classes
//
//=============================================================================
#ifndef BOUND_BOX_H
#define BOUND_BOX_H

#include "vector.h"

class CBoundBox
{
public:
	CBoundBox();
	~CBoundBox();

	void SetBounds(const CVector& min, const CVector& max);
	void ResetBounds();

	void UpdateBounds(const CVector& min, const CVector& max);
	void UpdateBounds(const CVector& point);
	void UpdateBounds(const CBoundBox& otherBox);

	void GetBoundsCentre(CVector& centre);
	void GetBoundsSize(CVector& size);

	bool ContainsPoint(const CVector& point);

	bool IsIntersectingBox(const CBoundBox& otherBox);
	bool IsIntersectingBox(const CVector& min, const CVector& max);
	bool IsInsideBox(const CBoundBox& otherBox);
	bool IsInsideBox(const CVector& min, CVector& max);	
	bool IsValid();
	
	CVector m_min; //The minimum point on the box
	CVector m_max; //The maximum point on the box
};

#endif