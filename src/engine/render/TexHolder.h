#ifndef TEX_HOLDER_H
#define TEX_HOLDER_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>
#include <string>
#include "engine/engine.h"

using namespace std;

class TexHolder
{
private:
	map<string,unsigned int> allDatas;
	inline static TexHolder * instance;
	unsigned int * noText;
	TexHolder()
	{
		noText = new unsigned int(0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

public:
	unsigned int GetTexture(string name)
	{
		if (allDatas.find(name) != allDatas.end())
			return allDatas[name];
		else
			return 0;
	}

	bool AddTexture(string name)
	{
		//Recuperation des infos de la texture
		unsigned int texture;
		int width, height, nrChannels;
		unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
		if (!data) return false;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//On peuple les données de la texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		allDatas[name] = texture;
		stbi_image_free(data);
		return true;
	}

	static TexHolder * GetInstance()
	{
		if (instance == NULL)
			instance = new TexHolder();
		return instance;
	}
};

#endif
