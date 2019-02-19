#ifndef GBUFFER_H
#define GBUFFER_H

#include "vbo.h"

class GBuffer
{

private:
	unsigned int gBuffer;
	unsigned int gLightPass;
	unsigned int gColor;
	unsigned int gDepth;
	int widthText;
	int heightText;

public:

	inline static GBuffer * _Instance;

	GBuffer()
	{
		glGenFramebuffers(1, &gBuffer);
		_Instance = this;
	}

	unsigned int getLightPass()
	{
		return gLightPass;
	}

	unsigned int getDepth()
	{
		return gDepth;
	}

	unsigned int getColor()
	{
		return gColor;
	}

	unsigned int getGBuffer()
	{
		return gBuffer;
	}

	void resize(int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, gColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, gLightPass);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	void Init(int width , int height)
	{
		widthText = width;
		heightText = height;

		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		glGenTextures(1, &gColor);
		glBindTexture(GL_TEXTURE_2D, gColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gColor, 0);

		glGenTextures(1, &gLightPass);
		glBindTexture(GL_TEXTURE_2D, gLightPass);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width ,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gColor, 0);

		glGenTextures(1, &gDepth);
		glBindTexture(GL_TEXTURE_2D, gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments);
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
