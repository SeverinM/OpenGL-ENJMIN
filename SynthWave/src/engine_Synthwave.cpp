#include "engine_synthwave.h"

void SynthEngine::init()
{
	//MRT
	GBuffer * postProcess;
	GBuffer * blur;

	//Camera
	Renderer->Camera->setPosition(YVec3f(10, 10, 10));
	Renderer->Camera->setLookAt(YVec3f(0, 0, 0));
	Renderer->setBackgroundColor(YColor(0, 0, 0, 1));
	
	//Config
	glEnable(GL_BLEND);

	//Vbos
	dec = new Decor();
	dec->GenerateGround(30, 30, 1);
}

void SynthEngine::update(float elapsed)
{
	YVec3f ZDir(Renderer->Camera->Direction);
	ZDir *= Z * 0.1;
	YVec3f XDir(Renderer->Camera->RightVec);
	XDir *= X * 0.1;

	Renderer->Camera->move(ZDir + XDir);
}

void SynthEngine::renderObjects()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	glUseProgram(shaderWorld);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(dec->getOffset().X, dec->getOffset().Y, dec->getOffset().Z);
	YRenderer::getInstance()->updateMatricesFromOgl();
	YRenderer::getInstance()->sendMatricesToShader(shaderWorld);
	dec->getGround()->render();
	glPopMatrix();
}