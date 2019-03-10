#include "engine_synthwave.h"
#include <ctime>

void SynthEngine::init()
{
	obj = new ObjImporter("obj/car.obj");
	obj->Initialize();

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
	
	string allNames[] = { "textures/right.png" , "textures/left.png" , "textures/top.png" , "textures/bottom.png" , "textures/back.png", "textures/front.png" };

	box = new SkyBox();
	box->Init(allNames);

	//Config
	glEnable(GL_BLEND);

	//Vbos
	borderTex = YTexManager::getInstance()->loadTextureFromDisk("textures/border.png");
	YTexManager::getInstance()->loadTextureToOgl(*borderTex);
	dec = new Decor((float)time(NULL));
	dec->GenerateGround(100, 100, 3,borderTex->Texture, YVec3f(-150,-150,0));
	dec->GenerateSun(box);
	dec->GenerateMountains(120, 10, 0.3, 3, YVec3f(-180, -180, 0), borderTex->Texture);
	dec->GenerateMountains(10, 100, 0.3, 3, YVec3f(-180, -150, 0), borderTex->Texture);
	dec->GenerateMountains(120, 10, 0.3, 3, YVec3f(-180, 150, 0), borderTex->Texture);
	dec->GenerateMountains(10, 100, 0.3, 3, YVec3f(150, -150, 0), borderTex->Texture);

	sunPos = YVec3f(10, 10, 10);
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
	glDisable(GL_CULL_FACE);
	ClearBuffer(bufferWorld);
	glEnable(GL_DEPTH_TEST);

	//On bouge la camera a l'origine
	YVec3f previousPos = Renderer->Camera->Position;
	Renderer->Camera->moveTo(YVec3f(0, 0, 0));
	glUseProgram(shaderSun);
	renderInTexture(bufferWorld, dec->getSun(),shaderSun);

	//glEnable(GL_CULL_FACE);
	//On la replace
	Renderer->Camera->moveTo(previousPos);
	Renderer->updateMatricesFromOgl();


	////Rendu FBO 1
	glUseProgram(shaderWorld); 
	glPushMatrix();
	glTranslatef(dec->getoriginGround().X, dec->getoriginGround().Y, dec->getoriginGround().Z);
	renderInTexture(bufferWorld, dec->getGround(), shaderWorld);
	glPopMatrix();
	
	for (std::pair<YVec3f, YVbo *> data : dec->getMountains())
	{
		glPushMatrix();
		glTranslatef(data.first.X, data.first.Y, data.first.Z);
		renderInTexture(bufferWorld, data.second, shaderWorld);
		glPopMatrix();
	}

	glUseProgram(shaderBasic);
	glUniform3f(glGetUniformLocation(shaderBasic, "sunPos"), 10, 10, 10);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 4, 0);
	renderInTexture(bufferWorld, obj->getVbo(), shaderBasic);	
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