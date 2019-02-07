#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine\engine.h"
#include "avatar.h"
#include "world.h"

class MEngineMinicraft : public YEngine {

public :
	YVbo * vbo;
	YVbo  * vbo2;
	unsigned int prog;
	float colors[3];
	SYSTEMTIME tm;
	SYSTEMTIME beginDay;
	float diff;
	unsigned int locationUniform;
	int hourOffset;
	int minOffset;
	int xMouse;
	int yMouse;
	bool RightHold;
	bool CtrlHold;
	bool wheelHold;
	int progWorld;
	float X;
	float Z;
	MAvatar * av;

	YColor * Day;
	YColor * Night;

	MWorld * wrld;

	//Gestion singleton
	static YEngine * getInstance()
	{
		if (Instance == NULL)
			Instance = new MEngineMinicraft();
		return Instance;
	}

	/*HANDLERS GENERAUX*/
	void loadShaders() {
		prog = Renderer->createProgram("shaders/sun");
		progWorld = Renderer->createProgram("shaders/world");
	}

	void init() 
	{
		X = 0;
		Z = 0;
		wrld = new MWorld();
		wrld->init_world(time(0));
		av = new MAvatar(Renderer->Camera, wrld);
		yMouse = -1;
		xMouse = -1;
		Day = new YColor(1, 1, 0.4f, 1);
		Night = new YColor(0, 0, 0.3f, 1);
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");
		GetLocalTime(&tm);

		glTranslatef(0.5f, -0.5f, 0);
		glPushMatrix();

		beginDay = tm;
		beginDay.wHour = 0;
		beginDay.wMinute = 0;
		beginDay.wSecond = 0;

		Renderer->setBackgroundColor(YColor(0.0f,0.0f,0.0f,1.0f));
		Renderer->Camera->setPosition(YVec3f(10, 10, 10));
		Renderer->Camera->setLookAt(YVec3f());

		vbo = new YVbo(3, 36, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);

		vbo->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo->setElementDescription(1, YVbo::Element(3)); //Normales
		vbo->setElementDescription(2, YVbo::Element(2)); //UV

		vbo->createVboCpu();
		
		float size(1);
		int count(0);

		vbo->SetFace(YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, count, true);
		vbo->SetFace(YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 0, 1), size, count, true);
		vbo->SetFace(YVec3f(0, 0, 0), YVec3f(0, 1, 0), YVec3f(0, 0, 1), size, count, true);

		vbo->SetFace(YVec3f(0, 0, size), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, count, true);
		vbo->SetFace(YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(-1, 0, 0), size, count, true);
		vbo->SetFace(YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(0, -1, 0), size, count, true);


		for (int i = 0; i < 36; i++)
		{
			vbo->setElementValue(1, i, 0.5f, 0.8f, 0.5f, 1);
		}

		vbo->createVboGpu();
		vbo->deleteVboCpu();

		count = 0;

		//vbo2
		vbo2 = new YVbo(3, 36, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);

		vbo2->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo2->setElementDescription(1, YVbo::Element(3)); //Normales
		vbo2->setElementDescription(2, YVbo::Element(2)); //UV

		vbo2->createVboCpu();

		vbo2->SetFace(YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, count, true);
		vbo2->SetFace(YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 0, 1), size, count, true);
		vbo2->SetFace(YVec3f(0, 0, 0), YVec3f(0, 1, 0), YVec3f(0, 0, 1), size, count, true);

		vbo2->SetFace(YVec3f(0, 0, size), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, count, true);
		vbo2->SetFace(YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(-1, 0, 0), size, count, true);
		vbo2->SetFace(YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(0, -1, 0), size, count, true);


		for (int i = 0; i < 36; i++)
		{
			vbo2->setElementValue(1, i, 0.5f, 0.8f, 0.5f, 1);
		}

		vbo2->createVboGpu();
		vbo2->deleteVboCpu();
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}

	void update(float elapsed) 
	{
		GetLocalTime(&tm);
		tm.wHour += hourOffset;
		tm.wHour %= 24;
		tm.wMinute += minOffset;
		tm.wMinute %= 60;
		YVec3f pos;
		pos = Renderer->Camera->RightVec;
		pos *= X;
		YVec3f posZ;
		posZ = Renderer->Camera->Direction;
		posZ *= Z;
		Renderer->Camera->move((pos + posZ).normalize() * 0.1f);
		av->update(0.015f);
	}

	void renderObjects() 
	{

		diff = (DiffTimeMs(tm, beginDay) % 86400) / 86400.0f;
		glUseProgram(0);
		//Rendu des axes
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(10000, 0, 0);
		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 10000, 0);
		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 10000);
		glEnd();

		Renderer->setBackgroundColor(Day->interpolateHSV(*Night,abs(diff - 0.5f) * 2));

		//Vbo
		glUseProgram(prog);
		locationUniform = glGetUniformLocation(prog, "sun_color");
		colors[2] = (tm.wSecond + (tm.wMilliseconds / 1000.0f));
		colors[1] = tm.wMinute + (colors[2] / 60.0f);
		colors[0] = tm.wHour + (colors[1] / 60.0f);

		colors[2] /= 60.0f;
		colors[1] /= 60.0f;
		colors[0] /= 24.0f;

		//Minutes
		glUniform3f(locationUniform, 1, abs(colors[1] - 0.5f) * 2,1);

		glPushMatrix();
		glRotatef((-360 * colors[1]) - 90, 0, 1, 0);
		glTranslatef(100, 0, 0);
		YRenderer::getInstance()->updateMatricesFromOgl();
		YRenderer::getInstance()->sendMatricesToShader(YRenderer::CURRENT_SHADER);
		vbo->render();
		glPopMatrix();

		glUseProgram(progWorld);
		wrld->render_world_vbo(true, false);
	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/
	void keyPressed(int key, bool special, bool down, int p1, int p2) 
	{
		if (key == 'g' && down)
		{
			minOffset += 5;
			while (minOffset > 60)
			{
				minOffset -= 60;
				hourOffset++;
			}
		}

		if ((key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) && down)
		{
			CtrlHold = true;
		}

		if ((key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) && !down)
		{
			CtrlHold = false;
		}

	
		/*if (key == 's')
		{
			Z += down ? -1 : 1;
		}

		if (key == 'z')
		{
			Z += down ? 1 : -1;
		}

		if (key == 'q')
		{
			X += down ? -1 : 1;
		}
		
		if (key == 'd')
		{
			X += down ? 1 : -1;
		}*/

		if (key == 's')
		{
			av->X += (down ? -1 : 1);
		}

		if (key == 'z')
		{
			av->X += (down ? 1 : -1);
		}

		if (key == 'q')
		{
			av->Z += (down ? -1 : 1);
		}

		if (key == 'd')
		{
			av->Z += (down ? 1 : -1);
		}

		if (key == 32 && down)
		{
			av->JumpAction();
		}
	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		if (dir > 0)
		{
			Renderer->Camera->IncrementFovY(1);
		}
		if (dir < 0)
		{
			Renderer->Camera->IncrementFovY(-1);
		}
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

	//x , y = position dans l'ecran
	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		showMouse(pressed);
		float deltaX(x - xMouse);
		float deltaY(y - yMouse);
		if (xMouse > 0 && yMouse > 0)
		{
			if (wheelHold)
			{
				Renderer->Camera->moveOnXY(-deltaX / 20.0f,-deltaY / 20.0f, CtrlHold);
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