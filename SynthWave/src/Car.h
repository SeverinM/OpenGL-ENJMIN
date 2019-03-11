#pragma once
#include "engine/render/VboIndex.h"
#include "engine/render/ObjImporter.h"

class Car
{
public:
	Car(string nameFile);
	void render(GLuint shader ,GLuint fbo = 0);

	inline void setPosition(float x, float y, float z, bool absolute = true)
	{
		if (absolute)
		{
			centerPosition = YVec3f(x, y, z);
		}
		else
		{
			centerPosition += YVec3f(x, y, z);
		}
	}

	inline YVec3f getPosition()
	{
		return centerPosition;
	}

	inline void setScale(float value)
	{
		scale = value;
	}

private:
	ObjImporter * obj;
	YVec3f centerPosition;
	YVec3f rotation;
	float scale;

};