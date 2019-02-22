#include "engine_synthwave.h"

void SynthEngine::init()
{
	//MRT
	bufferWorld = new GBuffer(2);
	bufferWorld->Init(800, 600);
	bufferBlur = new GBuffer(1);
	bufferBlur->Init(800, 600);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	glEnable(GL_DEPTH_TEST);
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

	//Rendu FBO 1
	glUseProgram(shaderWorld);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(dec->getOffset().X, dec->getOffset().Y, dec->getOffset().Z);
	renderInTexture(bufferWorld, dec->getGround());
	glPopMatrix();


	//Rendu FBO 2
	glUseProgram(shaderBlur);
	Renderer->sendMatricesToShader(shaderBlur);
	Renderer->sendTextureToShader(shaderPostPross, bufferWorld->getColorText(1), 0, "image");
	glDisable(GL_DEPTH_TEST);
	renderInScreenQuad(bufferBlur);

	//Vrai rendu
	glUseProgram(shaderPostPross);
	Renderer->sendTextureToShader(shaderPostPross, bufferWorld->getColorText(0), 0, "TexColor");
	Renderer->sendTextureToShader(shaderPostPross, bufferBlur->getColorText(0), 1, "TexBlurred");
	renderInScreenQuad();
}