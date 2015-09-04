//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: Implements functions for the player class
//
//=============================================================================
#if 0

#include "player.h"
#include "types/world_geometry.h"

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
	DetectWorldCollision();
}

void CPlayer::DetectWorldCollision()
{
	CVector rad_divider = CVector(PLAYER_RADIUS, PLAYER_HEIGHT, PLAYER_RADIUS);

	CVector pos = m_pos / rad_divider;
	CVector velocity = m_velocity / rad_divider;

	CollideWithWorld(pos, velocity, rad_divider);

	m_pos = pos *= rad_divider;
}

/**
Collides the player with the world. Done according to the sudocode provided on
http://cse125.ucsd.edu/2003/cse190g2/Collision.pdf
This is very TODO TODO TODO at the moment and could do with a LOT of cleanup
*/
void CPlayer::CollideWithWorld(CVector& point, CVector& velocity, const CVector& rad_divider)
{
	float distanceToTravel = velocity.GetLength();

	//If we don't have to travel far, just return now.
	if(distanceToTravel < 0.0001) return;

	//Work out all of the potential world objects we might collide with
	vector<CWorldGeometry*> potential_colliders;
	GetPotentialWorldColliders(m_boundbox, potential_colliders);

	//If there's nothing, then just exit now.
	if(potential_colliders.size() == 0)
	{
		point += velocity;
		return;
	}

	//Determining the nearest collision in all the potentially colliding objects
	bool collision_found = false;
	float nearest_distance = -1.0;
	CVector nearest_intersect_point = NULL;
	CVector nearest_polygon_intersect_point = NULL;

	int i, j;
	for(i = 0; i < potential_colliders.size(); i++) //looping all colliders
	{
		for(j = 0; j < potential_colliders[i]->GetSurfaceCount(); j++) //looping all surfaces
		{
			//Working out the plane of the current polygon
			CVector pOrigin = potential_colliders[i]->GetSurface(j).GetVertex(0);
			CVector pNormal = potential_colliders[i]->GetSurface(j).GetCachedNormal();

			//Scale collider's origin and normal to the squashed elipsoid space
			pOrigin /= rad_divider;
			pNormal /= rad_divider;

			//Distance from plane to point
			float pDist = RayPlaneIntersect(pOrigin, pNormal, point, -pNormal);
			CVector sphere_intersect_point;
			CVector plane_intersect_point;

			if(pDist < 0.0)
			{
				continue; //point is behind the plane, so ignore.
			}
			else if(pDist <= 1.0)
			{
				//Calculate plane intersect point
				plane_intersect_point = point + (-pNormal * pDist);
			}
			else
			{
				//Calculate sphere intersect point
				sphere_intersect_point = point - pNormal;
				CVector temp_velocity = velocity;
				temp_velocity.Normalise();
				float t = RayPlaneIntersect(pOrigin, pNormal, sphere_intersect_point, temp_velocity);

				//If travelling away from this polygon, ignore
				if(t < 0.0) continue;

				//Calculate plane intersect point
				CVector V = velocity * t;
				plane_intersect_point = sphere_intersect_point + V;
			}

			CVector polygon_intersect_point = plane_intersect_point;
			CVector closest_point_polygon;


			//polygon intersect point needs to be the same as plane intersect point
			if()
			{
				polygon_intersect_point = potential_colliders[i]->GetSurface(j).NearestPoint(plane_intersect_point);
			}

			CVector negative_velocity_vector = -velocity;

			//reverse intersect with the sphere
			float t = RaySphereIntersect(point, 1.0, polygon_intersect_point, negative_velocity_vector);

			if(t >= 0.0 && t <= distanceToTravel)
			{
				//Where do we intersect the sphere
				CVector intersect_point = polygon_intersect_point + negative_velocity_vector * t;

				//Closest intersect?

				if(!collision_found || t < nearest_distance)
				{
					nearest_distance = t;
					nearest_intersect_point = intersect_point;
					nearest_polygon_intersect_point = polygon_intersect_point;
					collision_found = true;
				}
			}
		}
	}

	//If we never have a collision, we can move there

	if(!collision_found)
	{
		point += velocity;
		return;
	}

	//Move to nearest collision -- START OF COLLISION RESOLUTION

	CVector V = velocity * (nearest_distance - 0.0001);
	point += V;

	//Destination relative to point of contact

	V.Normalise();
	V *= (distanceToTravel - nearest_distance);

	CVector destination_point = nearest_polygon_intersect_point + V;

	CVector slide_plane_origin = nearest_polygon_intersect_point;
	CVector slide_plane_normal = nearest_polygon_intersect_point - point;

	//Project destination onto sliding plane
	float time = RayPlaneIntersect(slide_plane_origin, slide_plane_normal, destination_point, slide_plane_normal);

	slide_plane_normal.Normalise();
	slide_plane_normal *= time;

	CVector destination_projection_normal = slide_plane_normal;
	CVector new_destination_point = destination_point + destination_projection_normal;

	//Create slide vector
	CVector new_velocity_vector = new_destination_point - nearest_polygon_intersect_point;

	CollideWithWorld(point, new_velocity_vector, CVector(1,1,1)); //from now on don't scale anything down
}
#endif