//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements a 3D vector class. It is up to the user whether this logically 
//			represents a point or a displacement in 3D space.
//
//=============================================================================
#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>

#define VECTOR_X 0
#define	VECTOR_Y 1
#define VECTOR_Z 2

class CVector
{
private:
	void init(); //common initialisation function.
	float length;	//the last computed length of this vector

public:
	float x;
	float y;
	float z;

	CVector(float *values); //set this vector to the values given in the 3 float array and sets whether this vector is a point in space or a vector
	CVector(float ix, float iy, float iz); //set this vector to the values given
	CVector();	//sets this vector to 0,0,0 

	~CVector();

	float GetLength();	//computes and returns the value for the length of this vector
	float GetCachedLength();	//returns the last computed value for the length of this vector
	float GetSQLength();	//computes and returns the value for the square length of this vector

	float DotProduct(const CVector& otherVec);	//performs the vector dot product: this . otherVec
	void CrossProduct(const CVector& otherVec, CVector& result);	//performs the vector cross product: this X otherVec -> result
	void SetArray(float* parray); //sets this vector to the given 3 float array
	float* GetArrayPtr() const; 
	void Normalise();	//normalises this vector, if it is a vector and not a point
	//void GetAngleBetween(const CVector &otherVec, CAngles &angles);
	void Random(float min, float max);
	void Negate();

	CVector operator+(const CVector& otherVec) const;
	CVector& operator+=(const CVector& otherVec);

	CVector operator-(const CVector& otherVec) const;
	CVector operator-(void);
	CVector& operator-=(const CVector& otherVec);

	CVector operator*(const CVector& otherVec) const;
	CVector operator*(float s);
	CVector& operator*=(const CVector& otherVec);
	CVector& operator*=(float s);

	CVector operator/(const CVector& otherVec) const;
	CVector operator/(float s);
	CVector& operator/=(const CVector& otherVec);
	CVector& operator/=(float s);

	bool operator==(const CVector& otherVec);
	bool operator!=(const CVector& otherVec);

	float& operator[](int i) const;
	CVector& operator=(const CVector& otherVec);
};

//-----------------------------------------------------------------------------
//Vector arithmetic operations
//-----------------------------------------------------------------------------

inline void VectorAdd(CVector const& a, CVector const& b, CVector& result)
{
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
}

inline void VectorSubtract(CVector const& a, CVector const& b, CVector& result)
{
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
}

inline void VectorMultiply(CVector const& a, CVector const& b, CVector& result)
{
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
}

inline void VectorMultiply(CVector const& a, float b, CVector& result)
{
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
}

inline void VectorDivide(CVector const& a, float b, CVector& result)
{
	assert(b != 0);
	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;
}

inline void VectorDivide(CVector const& a, CVector const& b, CVector& result)
{
	assert(b.x != 0 && b.y != 0 && b.z != 0);
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
}

/**
Steps the vector start in the direction of the vector dir by the percentage given in s.
e.g. start = 0,0,0 s = 0.5 dir = 1,0,0 result = 0.5, 0, 0
*/
inline void VectorMultiplyAdd(CVector const& start, float s, CVector const& dir, CVector result)
{
	result.x = start.x + s*dir.x;
	result.y = start.y + s*dir.y;
	result.z = start.z + s*dir.z;
}

/**
Computes the vector dor product in the form a . b
*/
inline float VectorDotProduct(CVector const& a, CVector const& b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

/**
Computes the vector cross product in the form result = a X b 
*/
inline void VectorCrossProduct(CVector const& a, CVector const& b, CVector& result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;	
}


/**
Interpolates between two vectors by the given percent. 
e.g. if source is 1,0,0 and source2 is 2,0,0 and percent is 0.5, destination will be 1.5,0,0.
*/
inline void VectorInterp(const CVector& source, const CVector& source2, float percent, CVector& destination)
{

	destination[VECTOR_X] = source[VECTOR_X] + (source2[VECTOR_X] - source[VECTOR_X]) * percent;
	destination[VECTOR_Y] = source[VECTOR_Y] + (source2[VECTOR_Y] - source[VECTOR_Y]) * percent;
	destination[VECTOR_Z] = source[VECTOR_Z] + (source2[VECTOR_Z] - source[VECTOR_Z]) * percent;
}

/**
Gets the distance between source and destination.
*/
inline float VectorDistanceBetween(const CVector& source, const CVector& destination)
{
	CVector delta(destination.x - source.x, destination.y - source.y, destination.z - source.z);

	return delta.GetLength();
}

/**
Gets the distance between source and destination in squared space.
*/
inline float VectorDistanceBetweenSQ(const CVector& source, const CVector& destination)
{
	CVector delta(destination.x - source.x, destination.y - source.y, destination.z - source.z);

	return delta.GetSQLength();
}

//-----------------------------------------------------------------------------
//Vector class operators
//-----------------------------------------------------------------------------

//Addition
inline CVector CVector::operator+(const CVector& otherVec) const
{

	CVector result;
	VectorAdd(*this, otherVec, result);
	return result;
}

inline CVector& CVector::operator+=(const CVector& otherVec)
{
	VectorAdd(*this, otherVec, *this);
	return *this;
}

//Subtraction
inline CVector CVector::operator-(const CVector& otherVec) const
{
	CVector result;
	VectorSubtract(*this, otherVec, result);
	return result;
}

inline CVector CVector::operator-(void)
{
	return CVector(-x, -y, -z);
}

inline CVector& CVector::operator-=(const CVector& otherVec)
{
	VectorSubtract(*this, otherVec, *this);
	return *this;
}

//Multiplication
inline CVector CVector::operator*(const CVector& otherVec) const
{
	CVector result;
	VectorMultiply(*this, otherVec, result);
	return result;

}

inline CVector CVector::operator*(float s)
{
	CVector result;
	VectorMultiply(*this, s, result);
	return result;
}

inline CVector& CVector::operator*=(const CVector& otherVec)
{
	VectorMultiply(*this, otherVec, *this);
	return *this;
}

inline CVector& CVector::operator*=(float s)
{
	VectorMultiply(*this, s, *this);

	return *this;
}

//Division
inline CVector CVector::operator/(const CVector& otherVec) const
{
	CVector result;
	VectorDivide(*this, otherVec, result);

	return result;
}

inline CVector CVector::operator/(float s)
{
	CVector result;
	VectorDivide(*this, s, result);
	
	return result;
}

inline CVector& CVector::operator/=(const CVector& otherVec)
{
	VectorDivide(*this, otherVec, *this);
	
	return *this;
}

inline CVector& CVector::operator/=(float s)
{
	VectorDivide(*this, s, *this);

	return *this;
}

//Equality
inline bool CVector::operator==(const CVector& otherVec)
{
	return(x == otherVec.x && y == otherVec.y && z == otherVec.z);
}

inline bool CVector::operator!=(const CVector& otherVec)
{
	return !(x == otherVec.x && y == otherVec.y && z == otherVec.z);
}

//Assignment
inline CVector& CVector::operator=(const CVector& otherVec)
{
	x = otherVec.x;
	y = otherVec.y;
	z = otherVec.z;

	return *this;
}

//Array access
inline float& CVector::operator[](int i) const
{
	assert(i > 0 && i < 3);
	return((float*)this)[i]; //cast this object to a float and read the x, y, z values as float pointers. Neat little trick.
}

#endif