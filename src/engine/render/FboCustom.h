#ifndef FBO_CUSTOM_H
#define FBO_CUSTOM_H
#include "engine/render/vbo.h"

class CustomFBO
{
	public :
		CustomFBO()
		{
			indexFBO = 0;
			indexTexture = 0;
			indexRBO = 0;
		}

		void init()
		{
			glGenFramebuffers(1, &indexFBO);
			glBindFramebuffer(GL_FRAMEBUFFER,indexFBO);
			glGenTextures(GL_TEXTURE_2D, &indexTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			//Lier FBO / Texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, indexTexture, 0);
			glGenRenderbuffers(1, &indexRBO);
			glBindRenderbuffer(GL_RENDERBUFFER, indexRBO);

			//Allocation memoire
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 800, 600);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			//Lier RBO / FBO
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT16, GL_RENDERBUFFER, indexRBO);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		unsigned int getFBO()
		{
			return indexFBO;
		}

		unsigned int getTexture()
		{
			return indexTexture;
		}

		unsigned int getRBO()
		{
			return indexRBO;
		}

	private:
		unsigned int indexFBO;
		unsigned int indexTexture;
		unsigned int indexRBO;
};

#endif
