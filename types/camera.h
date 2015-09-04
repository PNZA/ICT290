//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Camera class to represent a camera in 3D space.
//
//=============================================================================

#include "vector.h"

class CCamera
{
	float m_angle_yaw;
	float m_angle_pitch;

	CVector m_pos;
	CVector m_look_dir;
};