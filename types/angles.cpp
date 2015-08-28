//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Functions for the CAngle3D class
//
//=============================================================================

#include "angles.h"
#include "../math/mathlib.h"

void CAngles::init(){
	//do init stuff
}

CAngles::CAngles(float* values)
{
	pitch = values[AXIS_X];
	roll = values[AXIS_Z];
	yaw = values[AXIS_Y];
}
CAngles::CAngles(float ipitch, float iroll, float iyaw)
{
	pitch = ipitch;
	roll = iroll;
	yaw = iyaw;
}
CAngles::CAngles()
{
	pitch = roll = yaw = 0.0;
}

CAngles::~CAngles()
{
	//nothing to do
}

void CAngles::Random(float min, float max)
{
	pitch = min + ((float)rand() / RAND_MAX) * (max - min);
	roll = min + ((float)rand() / RAND_MAX) * (max - min);
	yaw = min + ((float)rand() / RAND_MAX) * (max - min);
}
void CAngles::Random(float min, float max, int axis)
{
	switch(axis){
		case AXIS_X: pitch = min + ((float)rand() / RAND_MAX) * (max - min); break;
		case AXIS_Y: roll = min + ((float)rand() / RAND_MAX) * (max - min); break;
		case AXIS_Z: yaw = min + ((float)rand() / RAND_MAX) * (max - min); break;
		default: assert(0);
	}
}

void CAngles::GetForward(CVector& vector)
{
	//TODO this needs testing
	vector.x = sin(DEG2RAD(yaw));
	vector.y = -(sin(DEG2RAD(pitch))*cos(DEG2RAD(yaw)));
	vector.z = -(cos(DEG2RAD(pitch))*cos(DEG2RAD(yaw)));
	vector.Normalise();
}


void CAngles::GetRotationMatrix()
{
	//create matrix here
	//matrix_4x4 matrix;
}


void CAngles::GetRotationMatrix()
{
	//to do rotation in one axis only
}

void CAngles::Normalise()
{
	int i;
	for(i = 0; i < 3; i++)
	{
		while(this[i] > 180.0)
			this[i] -= 360.0;

		while(this[i] < 180.0)
			this[i] += 360.0;
	}
}
/* looks to be kinda hard to implement
void CAngles::RotateAroundAxis()
{

}
*/

void CAngles::RotateAroundAxis(int axis, float degrees)
{
	switch(axis){
		case AXIS_X: pitch += degrees; break;
		case AXIS_Y: yaw += degrees; break;
		case AXIS_Z: roll += degrees; break;
		default: assert(0);
	}
	Normalise();
}

void CAngles::PointAt(const CVector& position, const CVector& point)
{
	CVector delta;
	delta = point - position;

	pitch = atan(delta.x / -(delta.y));
	yaw = atan(sqrt( delta.x*delta.x + delta.y*delta.y)/delta.z);
	//can't work out roll, so leave it as it was.
}