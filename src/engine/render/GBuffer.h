#ifndef GBUFFER_H
#define GBUFFER_H

#include "vbo.h"
#include <vector>

class GBuffer
{

private:
	unsigned int gBuffer;
	unsigned int gDepth;
	unsigned int rbo;
	std::vector<unsigned int *> allColorText;

	int widthText;
	int heightText;
	int nbText;

public:

	inline static GBuffer * _Instance;

	GBuffer(int nbColor)
	{
		glGenFramebuffers(1, &gBuffer);
		_Instance = this;
		nbText = nbColor;

		for (int i = 0; i < nbColor; i++)
		{
			allColorText.push_back(new unsigned int());
			glGenTextures(1, allColorText[i]);
		}
	}

	int getNbText()
	{
		return nbText;
	}

	unsigned int getColorText(int nb = 0)
	{
		return *allColorText[nb];
	}

	unsigned int getDepth()
	{
		return gDepth;
	}

	unsigned int getGBuffer()
	{
		return gBuffer;
	}

	unsigned int getRbo()
	{
		return rbo;
	}

	void resize(int width, int height)
	{
		for (unsigned int * text : allColorText)
		{
			glBindTexture(GL_TEXTURE_2D, *text);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Init(int width , int height)
	{
		widthText = width;
		heightText = height;

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		int i(0);
		for (unsigned int * text : allColorText)
		{
			glBindTexture(GL_TEXTURE_2D, *text);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, *text, 0);
			i++;
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		unsigned int * attachments = new unsigned int[i];
		for (int j = 0; j < i; j++)
		{
			attachments[j] = GL_COLOR_ATTACHMENT0 + j;
		}

		glDrawBuffers(i, attachments);
	}

	void RenderToTexture()
	{
	}

	void StopRendering()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

#endif GBUFFER_H
