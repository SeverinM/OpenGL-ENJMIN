#include "Car.h"
#include "engine/render/renderer.h"

Car::Car(string nameFile)
{
	obj = new ObjImporter(nameFile);
	obj->Initialize();

	centerPosition = YVec3f();
	rotation = YVec3f();
	scale = 1;
}

void Car::render(GLuint shader, GLuint fbo)
{
	glUseProgram(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glPushMatrix();
	glScalef(scale, scale, scale);
	glRotatef(-180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glTranslatef(centerPosition.X, centerPosition.Y, centerPosition.Z);
	YRenderer::getInstance()->updateMatricesFromOgl();
	YRenderer::getInstance()->sendMatricesToShader(shader);
	obj->render();
	glPopMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}