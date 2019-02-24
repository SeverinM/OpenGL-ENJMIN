#include "engine_synthwave.h"

void SynthEngine::init()
{
	//MRT
	bufferWorld = new GBuffer(3);
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

	string allNames[] = { "textures/right.png" , "textures/left.png" , "textures/top.png" , "textures/bottom.png" , "textures/back.png", "textures/front.png" };

	box = new SkyBox();
	box->Init(allNames);

	//Vbos
	dec = new Decor();
	dec->GenerateGround(20, 100, 3);
	dec->GenerateMountains(20, 20, 0.3, 3, YVec3f(0, 300, 0));
	dec->GenerateSun(box);
}

void SynthEngine::update(float elapsed)
{
	YVec3f ZDir(Renderer->Camera->Direction);
	ZDir *= Z;
	YVec3f XDir(Renderer->Camera->RightVec);
	XDir *= X;

	Renderer->Camera->move(ZDir + XDir);
}

void SynthEngine::renderObjects()
{
	ClearBuffer(bufferWorld);
	glEnable(GL_DEPTH_TEST);

	YVec3f previousPos = Renderer->Camera->Position;
	Renderer->Camera->moveTo(YVec3f(0, 0, 0));
	glUseProgram(shaderSun);
	Renderer->updateMatricesFromOgl();
	Renderer->sendMatricesToShader(shaderSun);
	renderInTexture(bufferWorld, dec->getSun());

	Renderer->Camera->moveTo(previousPos);
	Renderer->updateMatricesFromOgl();
	Renderer->sendMatricesToShader(shaderSun);


	//Rendu FBO 1
	glUseProgram(shaderWorld);
	glPushMatrix();
	glTranslatef(dec->getoriginGround().X, dec->getoriginGround().Y, dec->getoriginGround().Z);
	renderInTexture(bufferWorld, dec->getGround());
	glPopMatrix();
	glPushMatrix();
	glTranslatef(dec->getoriginMountains().X, dec->getoriginMountains().Y, dec->getoriginMountains().Z);
	renderInTexture(bufferWorld, dec->getMountains());
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
	Renderer->sendTextureToShader(shaderPostPross, bufferWorld->getColorText(2), 2, "Depth");
	renderInScreenQuad();
}