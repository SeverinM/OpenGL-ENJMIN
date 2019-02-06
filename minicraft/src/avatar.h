#ifndef __AVATAR__
#define __AVATAR__

#include "engine/utils/types_3d.h"
#include "engine/render/camera.h"
#include "engine/utils/timer.h"
#include "world.h"

class MAvatar
{
public:
	YVec3f Position;
	YVec3f Speed;

	bool Move;
	bool Jump;
	float Height;
	float CurrentHeight;
	float Width;
	bool avance;
	bool recule;
	bool gauche;
	bool droite;
	bool Standing;
	bool InWater;
	bool Crouch;
	bool Run;

	float DepthX;
	float DepthY;
	float DepthZ;

	YCamera * Cam;
	MWorld * World;

	YTimer _TimerStanding;

	MAvatar(YCamera * cam, MWorld * world)
	{
		Position = YVec3f(5,5,30);
		Height = 1.8f;
		CurrentHeight = Height;
		Width = 0.3f;
		Cam = cam;
		avance = false;
		recule = false;
		gauche = false;
		droite = false;
		Standing = false;
		Jump = true;
		World = world;
		InWater = false;
		Crouch = false;
		Run = false;
	}

	void update(float elapsed)
	{
		if (elapsed > 1.0f / 60.0f)
			elapsed = 1.0f / 60.0f;

		Speed += YVec3f(0, 0, -0.1f) * elapsed;	
		if (!Jump)
		{
			Speed = YVec3f(0, 0, 0);
		}
		Position += Speed * elapsed;
		World->getMinCol(Position, YVec3f(1,0,0), Width, Height, DepthX, false);
		World->getMinCol(Position, YVec3f(0, 1, 0), Width, Height, DepthY, false);
		World->getMinCol(Position, YVec3f(0, 0, 1), Width, Height, DepthZ, false);
		if (DepthZ < 0)
		{
			Position.Z -= DepthZ * 2;
			Jump = false;
		}
		Cam->moveTo(Position);
	}

	void JumpAction()
	{
		Speed += YVec3f(0, 0, 1);
		Jump = true;
	}
};

#endif