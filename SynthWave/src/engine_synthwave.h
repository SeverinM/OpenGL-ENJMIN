#ifndef SYNTH_ENGINE
#define SYNTH_ENGINE

#include "engine/engine.h"
#include "Decor.h"

class SynthEngine : public YEngine
{
	public:
		Decor * dec;

		//Shaders
		GLuint shaderPostPross;
		GLuint shaderBlur;
		GLuint shaderWorld;

		//Buffers
		GBuffer * bufferBlur;
		GBuffer * bufferPostProcess;

		//Input Handling
		int xMouse;
		int yMouse;
		bool RightHold;
		bool CtrlHold;
		bool wheelHold;
		int X;
		int Z;

		static YEngine * getInstance()
		{
			if (Instance == NULL)
				Instance = new SynthEngine();
			return Instance;
		}

		void loadShaders() {
			shaderWorld = Renderer->createProgram("shaders/SynthBase");
		}

		void init();
		void update(float elapsed);
		void renderObjects();

		void renderInTexture(GBuffer * buffer, GLuint shaderID, YVbo * source)
		{
			glUseProgram(shaderID);
			glBindFramebuffer(GL_FRAMEBUFFER, buffer->getGBuffer());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			source->render();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void resize(int width, int height)
		{

		}

		void keyPressed(int key, bool special, bool down, int p1, int p2)
		{
			if ((key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) && down)
			{
				CtrlHold = true;
			}

			if ((key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) && !down)
			{
				CtrlHold = false;
			}

			if (key == 's')
			{
				Z += (down ? -1 : 1);
			}

			if (key == 'z')
			{
				Z += (down ? 1 : -1);
			}

			if (key == 'q')
			{
				X += (down ? -1 : 1);
			}

			if (key == 'd')
			{
				X += (down ? 1 : -1);
			}
		}

		void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
		{

		}

		void mouseClick(int button, int state, int x, int y, bool inUi)
		{
			if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
			{
				RightHold = true;
			}

			if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
			{
				wheelHold = true;
			}

			if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
			{
				wheelHold = false;
			}

			if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
			{
				RightHold = false;
			}
		}

		void mouseMove(int x, int y, bool pressed, bool inUi)
		{
			showMouse(pressed);
			float deltaX(x - xMouse);
			float deltaY(y - yMouse);
			if (xMouse > 0 && yMouse > 0)
			{
				if (wheelHold)
				{
					Renderer->Camera->moveOnXY(-deltaX / 20.0f, -deltaY / 20.0f, CtrlHold);
				}
				else
				{
					if (RightHold)
					{
						if (CtrlHold)
						{
							Renderer->Camera->rotateAround(deltaX / 2.0f);
							Renderer->Camera->rotateUpAround(deltaY / 5.0f);
						}
						else
						{
							Renderer->Camera->rotate(deltaX / 2.0f);
							Renderer->Camera->rotateUp(deltaY / 5.0f);
						}
					}
				}

			}
			xMouse = x;
			yMouse = y;
		}
};

#endif