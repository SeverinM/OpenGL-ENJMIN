#pragma once

#include "engine/engine.h"
#include <string>

class SkyBox
{
private:
	GLuint indexCube;

public:

	GLuint GetCube()
	{
		return indexCube;
	}

	void Init(string textures[6]);
};