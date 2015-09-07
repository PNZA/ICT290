//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements functions for the player class
//
//=============================================================================
#if 1

#include "player.h"
#include "types/world_geometry.h"
#include "types/rayplane.h"

CPlayer::CPlayer()
{
	m_pos = CVector(0,0,0);
	m_camera_attach = PLAYER_CAMERA_ATTACH;
	m_velocity = CVector(0,0,0);

	m_radius = PLAYER_RADIUS;
	m_height = PLAYER_HEIGHT;

	//Initial bound box is 70 units wide (x,z) and 400 units tall (y).
	m_boundbox.SetBounds(CVector(-35, -200, -35), CVector(35, 200, 35));
	m_player_control_state = IN_AIR;
	
	//Initialise camera here
}

/**
Main player update function
*/
void CPlayer::Update(const SPlayerEventState& new_state)
{
	/*
	//Player update proceeds as follows
	//1. Determine where the player wants to be based on their player event state
	//2. Determine player restrictions (collision detection)
	//3. Apply player restrictions (collision resolution)
	//3. Update player state (update where the player is)

	//Do forward, backward, strafe key processing ONLY when on the ground (you can't move in the air silly, you're not a plane)
	if(m_player_control_state == ON_GROUND)
	{
		//TODO setting velocity to 0 when on the ground
		m_velocity.x = m_velocity.y = m_velocity.z = 0;

		CVector forward = m_camera.Forward();
		CVector right = m_camera.Right();

		forward.Normalise();
		right.Normalise();

		if(new_state.movement_buttons[PLAYER_KEY_FORWARD])
			VectorMultiplyAdd(m_velocity, PLAYER_MOVE_SPEED * FRAMERATE, forward, m_velocity);

		if(new_state.movement_buttons[PLAYER_KEY_BACKWARD])
			VectorMultiplyAdd(m_velocity, PLAYER_MOVE_SPEED * FRAMERATE, -forward, m_velocity);
		
		if(new_state.movement_buttons[PLAYER_KEY_RIGHT])
			VectorMultiplyAdd(m_velocity, PLAYER_MOVE_SPEED * FRAMERATE, right, m_velocity);
				
		if(new_state.movement_buttons[PLAYER_KEY_LEFT])
			VectorMultiplyAdd(m_velocity, PLAYER_MOVE_SPEED * FRAMERATE, -right, m_velocity);
	}
	else //m_player_control_state == IN_AIR 
	{
		//apply gravity in the air. The direction is positive because gravity is defined as a scalar negative
		VectorMultiplyAdd(m_velocity, GRAVITY * FRAMERATE, CVector(0,1,0), m_velocity);
	}

	//Determine player restrictions
	DoWorldCollision();
	*/
}

void CPlayer::DoWorldCollision()
{
	vector<CPolygon*> potential_colliders;
	GetPotentialWorldColliders(m_boundbox, potential_colliders);

	//we have no potential colliders, so don't worry.
	if(potential_colliders.size() == 0)
		m_pos += m_velocity;

	//if we do have some potential colliders, check collisions with them
	CollideWithWorld(potential_colliders);
}

CVector CPlayer::CollideWithWorld(vector<CPolygon*>& potential_colliders)
{
	return CVector();
}

void CPlayer::SetPos(const CVector& pos)
{
	m_boundbox.Translate(pos - m_pos);
	m_pos = pos;
}


#endif