#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"
#include "engine/render/ObjImporter.h"

#include "avatar.h"
#include "world.h"

class MEngineMinicraft : public YEngine {

public :
	YVbo * vbo;
	YVbo  * vbo2;
	unsigned int prog;
	float colors[3];
	SYSTEMTIME time;
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
	ObjImporter * obj;

	YColor * Day;
	YColor * Night;

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
	}

	void SetFace(YVbo * vbo, YVec3f origin, YVec3f directionFirst, YVec3f directionSecond, float size, int startingIndex, bool drawQuad = false , int nbList = 0)
	{
		YVec3f destination1 = (origin + (directionFirst.normalize() * size));
		YVec3f destination2 = (origin + (directionSecond.normalize() * size));

		vbo->setElementValue(nbList, startingIndex, origin.X, origin.Y, origin.Z);
		vbo->setElementValue(nbList, startingIndex + 1, destination1.X, destination1.Y, destination1.Z);
		vbo->setElementValue(nbList, startingIndex + 2, destination2.X, destination2.Y, destination2.Z);

		if (drawQuad)
		{
			YVec3f destination3 = origin + (directionFirst.normalize() * size) + (directionSecond.normalize() * size);
			vbo->setElementValue(nbList, startingIndex + 3, destination3.X, destination3.Y, destination3.Z);
			vbo->setElementValue(nbList, startingIndex + 4, destination2.X, destination2.Y, destination2.Z);
			vbo->setElementValue(nbList, startingIndex + 5, destination1.X, destination1.Y, destination1.Z);
		}
	}

	void init() 
	{
		obj = new ObjImporter("Handgun_obj.obj");
		obj->Initialize();

		yMouse = -1;
		xMouse = -1;
		Day = new YColor(1, 1, 0.4f, 1);
		Night = new YColor(0, 0, 0.3f, 1);
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");
		GetLocalTime(&time);

		glTranslatef(0.5f, -0.5f, 0);
		glPushMatrix();

		beginDay = time;
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

		SetFace(vbo, YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, 0, true);
		SetFace(vbo, YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 0, 1), size, 6, true);
		SetFace(vbo, YVec3f(0, 0, 0), YVec3f(0, 1, 0), YVec3f(0, 0, 1), size, 12, true);

		SetFace(vbo, YVec3f(0, 0, size), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, 18, true);
		SetFace(vbo, YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(-1, 0, 0), size, 24, true);
		SetFace(vbo, YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(0, -1, 0), size, 30, true);


		for (int i = 0; i < 36; i++)
		{
			vbo->setElementValue(1, i, 0.5f, 0.8f, 0.5f, 1);
		}

		vbo->createVboGpu();
		vbo->deleteVboCpu();

		//vbo2
		vbo2 = new YVbo(3, 36, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);

		vbo2->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo2->setElementDescription(1, YVbo::Element(3)); //Normales
		vbo2->setElementDescription(2, YVbo::Element(2)); //UV

		vbo2->createVboCpu();

		SetFace(vbo2, YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, 0, true);
		SetFace(vbo2, YVec3f(0, 0, 0), YVec3f(1, 0, 0), YVec3f(0, 0, 1), size, 6, true);
		SetFace(vbo2, YVec3f(0, 0, 0), YVec3f(0, 1, 0), YVec3f(0, 0, 1), size, 12, true);

		SetFace(vbo2, YVec3f(0, 0, size), YVec3f(1, 0, 0), YVec3f(0, 1, 0), size, 18, true);
		SetFace(vbo2, YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(-1, 0, 0), size, 24, true);
		SetFace(vbo2, YVec3f(size, size, size), YVec3f(0, 0, -1), YVec3f(0, -1, 0), size, 30, true);


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
		GetLocalTime(&time);
		time.wHour += hourOffset;
		time.wHour %= 24;
		time.wMinute += minOffset;
		time.wMinute %= 60;
	}

	void renderObjects() 
	{

		diff = (DiffTimeMs(time, beginDay) % 86400) / 86400.0f;
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
		colors[2] = (time.wSecond + (time.wMilliseconds / 1000.0f));
		colors[1] = time.wMinute + (colors[2] / 60.0f);
		colors[0] = time.wHour + (colors[1] / 60.0f);

		colors[2] /= 60.0f;
		colors[1] /= 60.0f;
		colors[0] /= 24.0f;

		//Secondes
		glUniform3f(locationUniform, 1, 1, abs(colors[2] - 0.5f) * 2);
		glPushMatrix();
		glRotatef((-360 * colors[2]) - 90, 0, 1, 0);
		glTranslatef(5,0,0);

		Renderer->updateMatricesFromOgl();
		Renderer->sendMatricesToShader(prog);
		vbo->render();

		glPopMatrix();

		//Minutes
		glUniform3f(locationUniform, 1, abs(colors[1] - 0.5f) * 2,1);

		glPushMatrix();
		glRotatef((-360 * colors[1]) - 90, 0, 1, 0);
		glTranslatef(3, 0, 0);

		Renderer->updateMatricesFromOgl();
		Renderer->sendMatricesToShader(prog);
		vbo->render();

		glPopMatrix();

		//Heures
		glUniform3f(locationUniform, abs(colors[0] - 0.5f) * 2, 1, 1);
		glPushMatrix();
		glRotatef((-360 * colors[0]) - 90, 0, 1, 0);
		glTranslatef(1, 0, 0);

		Renderer->updateMatricesFromOgl();
		Renderer->sendMatricesToShader(prog);
		vbo->render();

		glPopMatrix();
		glPushMatrix();

		Renderer->updateMatricesFromOgl();
		Renderer->sendMatricesToShader(prog);
		obj->render();

		glPopMatrix();
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

		if (key == 's')
		{
			Renderer->Camera->move(-Renderer->Camera->Direction.normalize());
		}

		if (key == 'z')
		{
			Renderer->Camera->move(Renderer->Camera->Direction.normalize());
		}

		if (key == 'q')
		{
			Renderer->Camera->move(-Renderer->Camera->RightVec.normalize());
		}
		
		if (key == 'd')
		{
			Renderer->Camera->move(Renderer->Camera->RightVec.normalize());
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