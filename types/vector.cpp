//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: functions for the CVector class.
//
//=============================================================================

#include "vector.h"
#include <stdlib.h> //rand
#include <math.h> //sqrt

//-----------------------------------------------------------------------------
//Constructors
//-----------------------------------------------------------------------------

void CVector::init()
{
	length = -1; //uninitialised length.
}
CVector::CVector(float *values)
{
	SetArray(values);
	init();
}

CVector::CVector(float ix, float iy, float iz)
{
	x = ix; y = iy, z = iz;
	init();
}

CVector::CVector()
{
	x = y = z = 0;
	init();
}

//-----------------------------------------------------------------------------
//Destructor
//-----------------------------------------------------------------------------
CVector::~CVector()
{
	//nothing to do
}

//-----------------------------------------------------------------------------
//Class functions
//-----------------------------------------------------------------------------

float CVector::GetLength()
{
	length = sqrt( x*x + y*y + z*z );
	return length;
}

float CVector::GetCachedLength() const
{
	return length;
}

float CVector::GetSQLength() const
{
	return (x*x + y*y + z*z); 
}

float CVector::DotProduct(const CVector& otherVec) const
{
	return VectorDotProduct(*this, otherVec);
}

void CVector::CrossProduct(const CVector& otherVec, CVector& result) const
{
	VectorCrossProduct(*this, otherVec, result);
}

void CVector::SetArray(float* parray)
{
	x = parray[VECTOR_X]; y = parray[VECTOR_Y]; z = parray[VECTOR_Z];
}

float* CVector::GetArrayPtr() const
{
	return (float*)this;
}

void CVector::Normalise()
{
	VectorDivide(*this, sqrt( x*x + y*y + z*z ), *this);
	length = 1.0; //TODO this might not be the beeest idea but technically this should be 1.0...
}

/** Might add this later if it's actually useful
void CVector::GetAngleBetween(const CVector &otherVec, CAngles &angles)
{

}
*/

void CVector::Random(float min, float max)
{
	x = min + ((float)rand() / RAND_MAX) * (max - min);
	y = min + ((float)rand() / RAND_MAX) * (max - min);
	z = min + ((float)rand() / RAND_MAX) * (max - min);
}

void CVector::Negate()
{
	x = -x; y = -y; z = -z;
}
