#pragma once
#include "vbo.h"


class YVboIndex : public YVbo 
{
private :
	GLuint VBOIndex;

public:
	void createVboGpu() override;
	void render(GBuffer * inBuffer = NULL) override;
	std::vector<GLuint> _Indices;
	YVboIndex (int nbElements, int nbVertices, DATA_STORAGE_METHOD storageMethod, bool index = false) : YVbo(nbElements,nbVertices, storageMethod,index){}
	void deleteVboCpu() override
	{
		YVbo::deleteVboCpu();
	}

	~YVboIndex()
	{
		SAFEDELETE_TAB(this->Elements);
		SAFEDELETE_TAB(ElementsValues);
		_Indices.clear();
		if (VAO != 0)
			glDeleteVertexArrays(1, &VAO);
		if (VBO != 0)
			glDeleteBuffers(1, &VBO);
		if (VBOIndex != 0)
			glDeleteBuffers(1, &VBOIndex);
	}
};