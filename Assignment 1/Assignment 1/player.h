//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: The player's class. Some parts of this may be moved to 
//			CBaseEntity later if needed.
//
//=============================================================================
#if 1
#ifndef PLAYER_H
#define PLAYER_H

#include "types/vector.h"
#include "types/bound_box.h"
#include "math/mathlib.h"
#include "math/physics.h"
#include <GL/freeglut.h>

#define PLAYER_RADIUS 50.0f
#define PLAYER_HEIGHT 300.0f

//This is in LOCAL coordinates!
#define PLAYER_CAMERA_ATTACH CVector(0, 150, -20)

//For determining whether any of the movement keys are held
#define PLAYER_KEY_FORWARD 0
#define PLAYER_KEY_BACKWARD 1
#define PLAYER_KEY_LEFT 2
#define PLAYER_KEY_RIGHT 3
#define PLAYER_KEY_JUMP 4

//For determining whether any of the mouse buttons are pressed.
#define PLAYER_MOUSE_LCLICK 0
#define PLAYER_MOUSE_RCLICK 1
#define PLAYER_MOUSE_MCLICK 2

#define IN_AIR 0
#define ON_GROUND 1

//Player moves 400 units per second
#define PLAYER_MOVE_SPEED 400

//Current frame button and mouse state.
struct SPlayerEventState
{
	bool movement_buttons[5];
	bool mouse_buttons[3];

	float mouse_x;
	float mouse_y;

	SPlayerEventState() //zero it all.
	{
		int i;
		for(i = 0; i < 5; i++)
		{
			movement_buttons[i] = false;
			if(i < 3) //you lazy ass
			{
				mouse_buttons[i] = false;
			}
		}
	}

	/*
	SPlayerEventState& GetCurrentEventState()
	{
		static SPlayerEventState current_state;
		return current_state;
	}

	SPlayerEventState& GetPreviousEventState()
	{
		static SPlayerEventState previous_state;
		return previous_state;
	}*/
};

class CPlayer
{

private:
	CVector m_pos;
	CVector m_camera_attach;
	CVector m_velocity;
	//CCamera m_camera;

	int m_player_control_state;

	float m_radius;
	float m_height;
	
	CBoundBox m_boundbox;
	void UpdateMovement();
	void UpdateCamera();

	void DoWorldCollision();
	//CollisionList CalculateColliders(vector<CPolygon*>& potential_colliders, const CVector& scaler, const CRay& r);
	CVector CollideWithWorld(vector<CPolygon*>& potential_colliders);
	void OnCollideWithWorld();
	
public:
	void CalculateColliders(vector<CPolygon*>& potential_colliders, const CVector& scaler, const CRay& r, CollisionList& colliders);
	CPlayer();
	~CPlayer(){};

	void Update(const SPlayerEventState& new_state); //Fully updates the player

	void SetPos(const CVector& pos); //Sets the players position
	const CVector& GetPos() {return m_pos;}; //Gets the players position
	const CVector& GetCameraAttach(); //Gets where the camera is on the player in world coordinates.
	const CVector& GetCameraAttachLocal(); //Gets where the camera is on the player in local coordinates.
	const CBoundBox& GetBoundBox() {return m_boundbox;};
	const CVector& GetVelocity() {return m_velocity;};

	float GetRadius();
	float GetHeight();
};

#endif
#endif