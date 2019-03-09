#include "VboIndex.h"
#include "renderer.h"

void YVboIndex::render(GBuffer * inBuffer)
{
	YRenderer::NbVBOFacesRendered += _Indices.size() / 3;

	glBindVertexArray(VAO);	

	glDrawElements(GL_TRIANGLES, _Indices.size(), GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
}

void YVboIndex::createVboGpu()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBOIndex);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, TotalSizeFloats * sizeof(float), ElementsValues, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Indices.size() * sizeof(GLuint), (void*)_Indices[0], GL_STATIC_DRAW);

	if (StorageMethod == PACK_BY_ELEMENT_TYPE) {
		for (int i = 0; i < NbElements; i++)
			glVertexAttribPointer(i, Elements[i].NbFloats, GL_FLOAT, GL_FALSE, 0, (void*)(Elements[i].OffsetFloats * sizeof(float)));
	}
	else {
		for (int i = 0; i < NbElements; i++)
			glVertexAttribPointer(i, Elements[i].NbFloats, GL_FLOAT, GL_FALSE, TotalNbFloatForOneVertice * sizeof(float), (void*)(Elements[i].OffsetFloats * sizeof(float)));
	}

	for (int i = 0; i < NbElements; i++)
		glEnableVertexAttribArray(i);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}