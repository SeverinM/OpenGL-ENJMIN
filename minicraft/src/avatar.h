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
		YVec3f moveDir(mouvZ + mouvX);

		Position += Speed * elapsed;

		World->getMinCol(Position, YVec3f(0, 0, 1), Width, Height, DepthZ, false,MWorld::AXIS_Z);
		if (DepthZ > 0 && DepthZ != 10000.0f && OnAir)
		{
 			Position.Z += DepthZ + 0.001f;
			Speed = YVec3f(0, 0, 0);
			OnAir = false;
		}

		Position += moveDir;
		World->getMinCol(Position, YVec3f(0, 0, 1), Width, Height, DepthY, false, MWorld::AXIS_Y);

		if (DepthY != 10000.0f)
			Position.Y -= moveDir.Y;

		World->getMinCol(Position, YVec3f(0, 0, 1), Width, Height, DepthX, false, MWorld::AXIS_X);
		if (DepthX != 10000.0f)
			Position.X -= moveDir.X;

		Cam->moveTo(Position);
	}

	void JumpAction()
	{
		if (OnAir) return;

		Speed += YVec3f(0, 0, 6);
		OnAir = true;
	}
};

#endif