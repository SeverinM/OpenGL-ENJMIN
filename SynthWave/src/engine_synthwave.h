#ifndef SYNTH_ENGINE
#define SYNTH_ENGINE

#include "engine/engine.h"
#include "Decor.h"
#include "engine/render/tex_manager.h"
#include "SkyBox.h"
#include "Car.h"

class SynthEngine : public YEngine
{
	public:

		//Sun
		Decor * dec;
		YVec3f sunPos;

		//Shaders
		GLuint shaderPostPross;
		GLuint shaderBlur;
		GLuint shaderWorld;
		GLuint shaderSun;
		GLuint shaderBasic;

		//Buffers
		GBuffer * bufferWorld;      
		GBuffer * bufferBlur;

		//Input Handling
		float xMouse;
		float yMouse;

		//SkyBox
		YTexFile * textures[6];
		YTexFile * borderTex;
		SkyBox * box;

		//VBO
		Car * car;
		YVbo * vbo;
		float speed;

		//Test
		GLuint VAO;
		GLuint VBOIndex;
		GLuint VBO;

		static YEngine * getInstance()
		{
			if (Instance == NULL)
				Instance = new SynthEngine();
			return Instance;
		}

		void loadShaders() {
			shaderWorld = Renderer->createProgram("shaders/SynthBase");
			shaderBlur = Renderer->createProgram("shaders/Blur");
			shaderPostPross = Renderer->createProgram("shaders/postprocess");
			shaderSun = Renderer->createProgram("shaders/Sun");
			shaderBasic = Renderer->createProgram("shaders/Basic");
		}

		void init();
		void update(float elapsed);
		void renderObjects();

		void renderInTexture(GBuffer * buffer, YVbo * source, GLuint shader)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, buffer->getGBuffer());
			YRenderer::getInstance()->updateMatricesFromOgl();
			YRenderer::getInstance()->sendMatricesToShader(shader);
			source->render();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void ClearBuffer(GBuffer * buffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, buffer->getGBuffer());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void renderInScreenQuad(GBuffer * buffer = NULL)
		{
			GLint shaderId;
			glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
			Renderer->sendNearFarToShader(shaderId);
			Renderer->sendScreenSizeToShader(shaderId);
			Renderer->sendMatricesToShader(shaderId);
			glBindFramebuffer(GL_FRAMEBUFFER, buffer ? buffer->getGBuffer() : 0);
			Renderer->drawFullScreenQuad();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void resize(int width, int height)
		{
			bufferWorld->resize(width, height);
			bufferBlur->resize(width, height);
		}

		void mouseMove(int x, int y, bool pressed, bool inUi)
		{
			showMouse(pressed);
			float deltaX(x - xMouse);
			float deltaY(y - yMouse);
			if (xMouse > 0 && yMouse > 0)
			{
				Renderer->Camera->rotateAround(deltaX / 2.0f);
				Renderer->Camera->rotateUpAround(deltaY / 5.0f);
			}
			xMouse = x;
			yMouse = y;
		}
};

#endif
