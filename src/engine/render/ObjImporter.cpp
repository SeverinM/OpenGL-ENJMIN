#include "ObjImporter.h"
#include <fstream>
#include <vector>
#include "OBJ_Loader.h"

ObjImporter::ObjImporter(string fileName)
{
	nameFile = fileName;
	countVertices = 0;
	countIndex = 0;
	countNormal = 0;
	nbVertices = 0;
	nbIndex = 0;
}

bool ObjImporter::Initialize()
{
	int index(0);
	int indexInd(0);

	objl::Loader loader;
	if (loader.LoadFile(nameFile))
	{
		vbo = new YVbo(2, loader.LoadedVertices.size() , YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
		vboIndex = new YVbo(2, loader.LoadedIndices.size(), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, true);
		vbo->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo->setElementDescription(1, YVbo::Element(3)); //Normales
		vboIndex->setElementDescription(1, YVbo::Element(1)); //Index Normales
		vboIndex->setElementDescription(0, YVbo::Element(1)); //Index Position
		vbo->createVboCpu();
		vboIndex->createVboCpu();

		for (objl::Vertex vertex : loader.LoadedVertices)
		{
			vbo->setElementValue(0, index, vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
			vbo->setElementValue(1, index, vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
			index++;
		}

		for (GLuint ind : loader.LoadedIndices)
		{
			vboIndex->setElementValue(0, indexInd, ind);
			vboIndex->setElementValue(1, indexInd, ind);
			indexInd++;
		}
	}
	else
	{
		std::cout << "error" << std::endl;
	}

	vbo->createVboGpu(vboIndex);
	vbo->deleteVboCpu();
	vboIndex->deleteVboCpu();

	return true;
}

