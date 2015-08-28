//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Math library containing math functions
//
//=============================================================================

#ifndef MATHLIB_H
#define MATHLIB_H

#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846



//Converting between degrees and radians
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / PI) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(PI / 180.f) )

//Used to specify premultiplication or postmultiplication
enum{
	MATRIX_PRE,
	MATRIX_POST
}

/** not needed yet
class CMatrix4x4
{
private:
	float matrix[4][4];
public:

};
*/


#endif