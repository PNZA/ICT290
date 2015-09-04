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

	void GetBoundsCentre(CVector& centre) const;
	void GetBoundsSize(CVector& size) const;

	bool ContainsPoint(const CVector& point) const;

	void Translate(const CVector& direction);

	bool IsIntersectingBox(const CBoundBox& otherBox) const;
	bool IsIntersectingBox(const CVector& min, const CVector& max) const; 
	bool IsInsideBox(const CBoundBox& otherBox) const;
	bool IsInsideBox(const CVector& min, CVector& max) const;	
	bool IsValid() const;
	
	CVector m_min; //The minimum point on the box
	CVector m_max; //The maximum point on the box
};

#endif