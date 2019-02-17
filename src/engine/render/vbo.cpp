#include "engine/render/vbo.h" 
#include "engine/render/renderer.h"
#include "engine/engine.h"

//Il ne faut pas le vbo soit deja crée
void YVbo::createVboGpu(YVbo * index) {
	
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	YLog::log(YLog::ENGINE_INFO, (string("Creation VAO ") + toString(VAO)).c_str());

	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
	glGenBuffers(1, &VBO);

	if (index != NULL && index->getIndexVBO() == 0)
	{
		glGenBuffers(1, index->getIndexVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(index->getIndexVBO()));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index->getVboSizeBytes(), index->getElement(0), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	YLog::log(YLog::ENGINE_INFO, (string("Creation VBO ") + toString(VBO)).c_str());
	std::cout << TotalSizeFloats << std::endl;

	//On alloue et copie les datas
	glBufferData(GL_ARRAY_BUFFER,
		TotalSizeFloats * sizeof(float),
		ElementsValues,
		GL_STATIC_DRAW);

	YRenderer::checkGlError("glBufferData");

	//On debind
	if (index != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void YVbo::render(GBuffer * inBuffer) {

	//La stat globales
	YRenderer::NbVBOFacesRendered += NbVertices / 3;

	if (textureIndex) glBindTexture(GL_TEXTURE_2D, textureIndex);
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
	if (inBuffer)
		inBuffer->RenderToTexture();
	else
		glDrawArrays(GL_TRIANGLES, 0, NbVertices);
	YEngine::Instance->TimerGPURender.endAccumPeriod();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}