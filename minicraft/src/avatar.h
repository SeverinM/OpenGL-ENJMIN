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
	bool Standing;
	bool InWater;
	bool Crouch;
	bool Run;
	bool OnAir;

	float DepthX;
	float DepthY;
	float DepthZ;
	float MoveSpeed;
	int X;
	int Z;

	YCamera * Cam;
	MWorld * World;

	YTimer _TimerStanding;

	MAvatar(YCamera * cam, MWorld * world)
	{
		X = 0;
		Z = 0;
		MoveSpeed = 5.2f;
		Position = YVec3f(5,5,20);
		Height = 1.0f;
		CurrentHeight = Height;
		Width = 0.3f;
		Cam = cam;
		Standing = false;
		Jump = true;
		World = world;
		InWater = false;
		Crouch = false;
		Run = false;
		OnAir = true;
	}

	void MovePos(float elapsed, YVec3f direction)
	{
		direction.Z = 0;
		Position += direction * elapsed;
	}

	void update(float elapsed)
	{
		float previousPos(Position.Z);
		//Application des forces
		if (elapsed > 1.0f / 60.0f)
			elapsed = 1.0f / 60.0f;

		if (OnAir)
		{
			Speed += World->_Gravity * elapsed;
		}

		//Application des mouvemennts
		YVec3f mouvZ(Cam->Direction.X, Cam->Direction.Y, 0);
		YVec3f mouvX(Cam->RightVec.X, Cam->RightVec.Y, 0);
		mouvZ = mouvZ.normalize();
		mouvX = mouvX.normalize();
		mouvZ *= elapsed * X * MoveSpeed;
		mouvX *= elapsed * Z * MoveSpeed;

		OnAir = (DepthZ > 0.05f);
		Position += Speed * elapsed;
		World->getMinCol(Position, YVec3f(0, 0, 1), Width, Height, DepthZ, false);
		if (DepthZ < 0)
		{
			Position.Z -= DepthZ + 0.001f;
			Speed.Z = 0;
			Jump = false;
			OnAir = false;
		}

		World->getMinCol(Position + (Speed * elapsed), YVec3f(1, 0, 0), Width, Height, DepthX, false);
		World->getMinCol(Position + (Speed * elapsed), YVec3f(0, 1, 0), Width, Height, DepthY, false);
		Position += mouvX + mouvZ;
		Cam->moveTo(Position);
	}

	void JumpAction()
	{
		if (Jump) return;

		Speed += YVec3f(0, 0, 2);
		Jump = true;
	}
};

#endif