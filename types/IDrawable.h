//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Interface for a drawable object
//
//=============================================================================

#ifndef IDRAWABLE_H
#define IDRAWABLE_H

class IDrawable
{
public:
	virtual void Draw();
	virtual bool ShouldDraw();
	virtual ~IDrawable() {};
};

#endif