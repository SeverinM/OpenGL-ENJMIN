#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#define NB_SLIDER 10
#include "engine\engine.h"
#include "avatar.h"
#include "world.h"
#include "engine/render/TexHolder.h"
#include "engine/render/GBuffer.h"
#include <ctime>

class MEngineMinicraft : public YEngine {

public :
	YVbo * vbo;
	YVbo  * vbo2;
	unsigned int prog;
	float colors[3];
	SYSTEMTIME tm;
	SYSTEMTIME beginDay;
	float diff;

	//Shaders
	unsigned int locationUniform;
	unsigned int postPross;
	unsigned int progWorld;
	unsigned int progBlur;

	int hourOffset;
	int minOffset;
	int xMouse;
	int yMouse;
	bool RightHold;
	bool CtrlHold;
	bool wheelHold;
	float X;
	float Z;
	clock_t diffTime;
	MAvatar * av;
	int fps;
	GLuint textIndex;

	//Buffers intermediaires
	GBuffer * gBuff;
	//GBuffer * buffBlur;

	YColor * Day;
	YColor * Night;
	GUISlider * sliders[NB_SLIDER];

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
		postPross = Renderer->createProgram("shaders/postprocess");
		progBlur = Renderer->createProgram("shaders/Blur");
	}

	void init() 
	{
		gBuff = new GBuffer(2);
		gBuff->Init(Renderer->ScreenWidth, Renderer->ScreenHeight);

		//buffBlur = new GBuffer(1);
		//buffBlur->Init(Renderer->ScreenWidth, Renderer->ScreenHeight);

		TexHolder::GetInstance()->AddTexture("textures/normal.jpg");
		textIndex = TexHolder::GetInstance()->GetTexture("textures/normal.jpg");

		fps = 60;
		diff = 0.015f;
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
		glEnable(GL_DEPTH_TEST);

		for (int i = 0; i < 10; i++)
		{
			sliders[i] = new GUISlider();
			sliders[i]->setPos(20, 30 * i + 50);
			addToScreenParam(sliders[i]);
		}

		Renderer->setBackgroundColor(YColor(0, 0, 0, 1));
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

		av->update(YEngine::DeltaTime);
	}

	void renderObjects() 
	{
		YLog::getInstance()->Update(DeltaTime);

		diff = (DiffTimeMs(tm, beginDay) % 86400) / 86400.0f;
		glUseProgram(0);
		//Rendu des axes
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		YLog::getInstance()->Render();
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
		//vbo->render();
		glPopMatrix();

		YVec3f fictifPosition(500, 500, 100);

		glUseProgram(progWorld);
		Renderer->sendTimeToShader(YEngine::getInstance()->DeltaTimeCumul, progWorld);
		for (int i = 0; i < 10; i++)
		{
			string varName = "slider_" + toString(i);
			unsigned int uniform = glGetUniformLocation(progWorld, varName.c_str());
			glUniform1f(uniform, sliders[i]->Value);
		}
		unsigned int uniform = glGetUniformLocation(progWorld, "camPos");
		float x(Renderer->Camera->Position.X);
		float y(Renderer->Camera->Position.Y);
		float z(Renderer->Camera->Position.Z);
		glUniform3f(uniform, x, y, z);
		uniform = glGetUniformLocation(progWorld, "sunPos");
		glUniform3f(uniform,fictifPosition.X,fictifPosition.Y,fictifPosition.Z);
		uniform = glGetUniformLocation(progWorld, "skyColor");
		glUniform3f(uniform, Day->interpolateHSV(*Night, abs(diff - 0.5f) * 2).R, Day->interpolateHSV(*Night, abs(diff - 0.5f) * 2).V, Day->interpolateHSV(*Night, abs(diff - 0.5f) * 2).B);
		uniform = glGetUniformLocation(progWorld, "world_size");
		glUniform2f(uniform, MWorld::MAT_SIZE * MWorld::MAT_SIZE_CUBES, MWorld::MAT_SIZE * MWorld::MAT_SIZE_CUBES);

		YRenderer::getInstance()->updateMatricesFromOgl();
		YRenderer::getInstance()->sendMatricesToShader(YRenderer::CURRENT_SHADER);

		//Rendu monde
		glBindFramebuffer(GL_FRAMEBUFFER, gBuff->getGBuffer());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		wrld->render_world_vbo(true, false);

		//Rendu FBO 1 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(progBlur);
		glUniform1i(glGetUniformLocation(progBlur, "image"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuff->getColorText(1));
		glDisable(GL_DEPTH_TEST);

		//Rendu FBO 2
		/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(postPross);
		glUniform1i(glGetUniformLocation(postPross, "TexColor"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuff->getColorText(0));
		glDisable(GL_DEPTH_TEST);*/

		Renderer->sendNearFarToShader(progBlur);
		Renderer->sendScreenSizeToShader(progBlur);
		Renderer->sendMatricesToShader(progBlur);
		Renderer->drawFullScreenQuad();
	}

	void resize(int width, int height) {
		gBuff->resize(width, height);
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

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			wrld->RemoveCube(Renderer->Camera->Direction, Renderer->Camera->Position);
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