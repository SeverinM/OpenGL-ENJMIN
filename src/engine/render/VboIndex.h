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
};