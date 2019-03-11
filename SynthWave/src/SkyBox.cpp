#include "SkyBox.h"
#include "engine/render/tex_manager.h"

void SkyBox::Init(string textures[6])
{
	glGenTextures(1, &indexCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, indexCube);

	for (int i = 0; i < 6; i++)
	{
		YTexFile * file = YTexManager::getInstance()->loadTexture(textures[i]);
		YTexManager::getInstance()->loadTexCube(*file, i);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}