#include "engine/render/vbo.h" 
#include "engine/render/renderer.h"
#include "engine/engine.h"

//Il ne faut pas le vbo soit deja crée
void YVbo::createVboGpu() {
	
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	YLog::log(YLog::ENGINE_INFO, (string("Creation VAO ") + toString(VAO)).c_str());

	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//On alloue et copie les datas
	glBufferData(GL_ARRAY_BUFFER,
		TotalSizeFloats * sizeof(float),
		ElementsValues,
		GL_STATIC_DRAW);

	YLog::log(YLog::ENGINE_INFO, (string("Creation VBO ") + toString(VBO)).c_str());
	YRenderer::checkGlError("glBufferData");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void YVbo::render(GBuffer * inBuffer) {

	//La stat globales
	YRenderer::NbVBOFacesRendered += NbVertices / 3;

	if (textureIndex)
	{
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}

	if (textureCubeIndex)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeIndex);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for (int i = 0; i<NbElements; i++)
		glEnableVertexAttribArray(i);

	if (StorageMethod == PACK_BY_ELEMENT_TYPE) {
		for (int i = 0; i<NbElements; i++)
			glVertexAttribPointer(i, Elements[i].NbFloats, GL_FLOAT, GL_FALSE, 0, (void*)(Elements[i].OffsetFloats * sizeof(float)));
	} else {
		for (int i = 0; i<NbElements; i++)
			glVertexAttribPointer(i, Elements[i].NbFloats, GL_FLOAT, GL_FALSE, TotalNbFloatForOneVertice * sizeof(float), (void*)(Elements[i].OffsetFloats * sizeof(float)));
	}

	YEngine::Instance->TimerGPURender.startAccumPeriod();
	glDrawArrays(GL_TRIANGLES, 0, NbVertices);
	YEngine::Instance->TimerGPURender.endAccumPeriod();
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}