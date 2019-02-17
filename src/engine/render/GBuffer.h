#ifndef GBUFFER_H
#define GBUFFER_H

#include "vbo.h"

class GBuffer
{

private:
	unsigned int gBuffer;
	unsigned int gLightPass;
	unsigned int gColor;

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

	unsigned int getColor()
	{
		return gColor;
	}

	unsigned int getGBuffer()
	{
		return gBuffer;
	}

	void Init(int width , int height)
	{
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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
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
