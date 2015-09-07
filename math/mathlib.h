//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Math library containing math functions
//
//=============================================================================

#ifndef MATHLIB_H
#define MATHLIB_H
#include "../types/vector.h"

#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846
#define TWOPI 6.28318530717958647692

//66 updates per second. Note this is not render frames, this is phys/logic updates
static float FRAMERATE = 1.0f/66.0f;

static CVector ORIGIN = CVector(0,0,0); //Maybe we can set the origin elsewhere sometime?

//Converting between degrees and radians
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / PI) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(PI / 180.f) )

//Used to specify premultiplication or postmultiplication
enum{
	MATRIX_PRE,
	MATRIX_POST
};

/** not needed yet
class CMatrix4x4
{
private:
	float matrix[4][4];
public:

};
*/

/**
Convert a point defined in local space around loc_origin to a point defined in world space.
*/
inline void LocalToWorld(const CVector& loc_origin, const CVector& loc_pos, CVector& world_pos)
{
	world_pos = loc_origin + loc_pos;
}

/**
Convert a point defined in world space to a point defined in local space around loc_origin.
*/
inline void WorldToLocal(const CVector& loc_origin, const CVector& pos, CVector& loc_pos)
{
	loc_pos = pos - loc_origin;
}

#endif