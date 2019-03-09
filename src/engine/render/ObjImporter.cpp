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
		vbo = new YVboIndex(2, loader.LoadedIndices.size(), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
		vbo->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo->setElementDescription(1, YVbo::Element(3)); //Normale
		vbo->createVboCpu();

		for (GLuint ind : loader.LoadedIndices)
		{
			vbo->_Indices.push_back(ind);
		}

		for (objl::Vertex vert : loader.LoadedVertices)
		{
			vbo->setElementValue(0, index, vert.Position.X, vert.Position.Y, vert.Position.Z);
			vbo->setElementValue(1, index, vert.Normal.X, vert.Normal.Y, vert.Normal.Z);
			index++;
		}
	}
	else
	{
		std::cout << "error" << std::endl;
	}

	vbo->createVboGpu();
	vbo->deleteVboCpu();

	return true;
}

