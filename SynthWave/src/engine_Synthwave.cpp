#include "engine_synthwave.h"
#include <ctime>

void SynthEngine::init()
{
	car = new Car("obj/car.obj");
	car->setPosition(0, 1.5, 0, false);
	speed = 2;

	//MRT
	bufferWorld = new GBuffer(3);
	bufferWorld->Init(800, 600);

	bufferBlur = new GBuffer(1);
	bufferBlur->Init(800, 600);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Camera
	Renderer->Camera->moveTo(-car->getPosition() + YVec3f(0, 10, 10));
	Renderer->Camera->setLookAt(-car->getPosition() + YVec3f(0,0,2));
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
	dec->GenerateGround(40, 40, 3,borderTex->Texture, YVec3f(-60,-60,0));
	dec->GenerateSun(box);
	dec->GenerateMountains(10, 40, 0.3, 3, YVec3f(-90, -60, 0), borderTex->Texture);
	dec->GenerateMountains(10, 40, 0.3, 3, YVec3f(60, -60, 0), borderTex->Texture);

	sunPos = YVec3f(10, 10, 10);
}

void SynthEngine::update(float elapsed)
{
	car->setPosition(0,0,DeltaTime * speed, false);
	Renderer->Camera->move(YVec3f(0,DeltaTime * speed,0));
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

	//Rendu voiture
	glUseProgram(shaderBasic);
	glUniform3f(glGetUniformLocation(shaderBasic, "sunPos"), 10, 10, 10);
	car->render(shaderBasic, bufferWorld->getGBuffer());

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