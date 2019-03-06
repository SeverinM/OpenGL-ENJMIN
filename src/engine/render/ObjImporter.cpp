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
		vbo = new YVbo(2, loader.LoadedIndices.size(), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
		vbo->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo->setElementDescription(1, YVbo::Element(3)); //Normales
		vbo->createVboCpu();

		for (GLuint ind : loader.LoadedIndices)
		{
			objl::Vector3 sampled = loader.LoadedVertices[ind].Position;
			objl::Vector3 sampledNorm = loader.LoadedVertices[ind].Normal;
			vbo->setElementValue(0, index, sampled.X, sampled.Y, sampled.Z);
			vbo->setElementValue(1, index, sampledNorm.X, sampledNorm.Y, sampledNorm.Z);
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

