//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements a 3D angles class containing pitch, roll and yaw. Used to
//			orientate geometry in the world. Angles are in degrees. 
//
//=============================================================================

#ifndef ANGLES_H
#define ANGLES_H

#include <assert.h>
#include "vector.h"

#define ANGLES_PITCH 0
#define ANGLES_ROLL 1
#define ANGLES_YAW 2

#define AXIS_X 0
#define AXIS_Y 2
#define AXIS_Z 1

class CAngles
{
private:
	void init();

public:
	float pitch; //x axis rotation
	float roll; //z axis rotation
	float yaw; //y axis rotation

	CAngles(float* values);
	CAngles(float ipitch, float iroll, float iyaw);
	CAngles();

	~CAngles();

	void Random(float min, float max);
	void Random(float min, float max, int axis);

	void GetForward(CVector& vector);

	void GetRotationMatrix();
	void GetRotationMatrix(int axis);

	void Normalise();
	//void RotateAroundAxis(const CVector& axis, float degrees);
	void RotateAroundAxis(int axis, float degrees);
	void PointAt(const CVector& position, const CVector& point);

	CAngles operator+(const CAngles& otherAngle) const;
	CAngles& operator+=(const CAngles& otherAngle);
	CAngles operator-(const CAngles& otherAngle) const;
	CAngles operator-(void);
	CAngles& operator-=(const CAngles& otherAngle);
	bool operator==(const CAngles& otherAngle);
	bool operator!=(const CAngles& otherAngle);
	float& operator[](int i) const;
};

//-----------------------------------------------------------------------------
//Angle arithmetic
//-----------------------------------------------------------------------------
inline void AngleAdd(const CAngles& a, const CAngles& b, CAngles& result)
{
	result.pitch = a.pitch + b.pitch;
	result.roll = a.roll + b.roll;
	result.yaw = a.yaw + b.yaw ;
}

inline void AngleSubtract(const CAngles& a, const CAngles& b, CAngles& result)
{
	result.pitch = a.pitch - b.pitch;
	result.roll = a.roll - b.roll;
	result.yaw = a.yaw - b.yaw ;
}

inline float AngleBetween(const CAngles& a, const CAngles& b, int axis)
{
	switch(axis){
		case AXIS_X: return a[AXIS_X] - b[AXIS_X]; break;
		case AXIS_Y: return a[AXIS_Y] - b[AXIS_Y]; break;
		case AXIS_Z: return a[AXIS_Z] - b[AXIS_Z]; break;
		default: assert(0);
	}
}

//-----------------------------------------------------------------------------
//Angle operators
//-----------------------------------------------------------------------------
inline CAngles CAngles::operator+(const CAngles& otherAngle) const
{
	CAngles result;	
	AngleAdd(*this, otherAngle, result);
	return result;
}

inline CAngles& CAngles::operator+=(const CAngles& otherAngle)
{
	AngleAdd(*this, otherAngle, *this);
	return *this;
}

inline CAngles CAngles::operator-(const CAngles& otherAngle) const
{
	CAngles result;
	AngleSubtract(*this, otherAngle, result);
	return result;
}

inline CAngles CAngles::operator-(void)
{
	return CAngles(-pitch, -roll, -yaw);
}

inline CAngles& CAngles::operator-=(const CAngles& otherAngle)
{
	AngleSubtract(*this, otherAngle, *this);
	return *this;
}

inline float& CAngles::operator[](int i) const
{
	assert(i > 0 && i < 3);
	return ((float*)this)[i]; //cast this object to a float and read the pitch, roll, yaw values as float pointers.
}

#endif