#include "ObjImporter.h"
#include <fstream>
#include <vector>

ObjImporter::ObjImporter(string fileName)
{
	nameFile = fileName;
	countVertices = 0;
	countIndex = 0;
	countNormal = 0;
	nbVertices = 0;
}

bool ObjImporter::Initialize()
{
	ifstream file;
	string line;
	file.open(nameFile);

	if (file)
	{
		//On lit une premiere fois pour connaitre le nombre de vertices
		while (std::getline(file, line))
		{
			string pref = line.substr(0, 2);
			if (pref == "v ")
			{
				nbVertices++;
			}
		}

		file.clear();
		file.seekg(0);
		vbo = new YVbo(2, nbVertices, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
		vboIndex = new YVbo(2, nbVertices * 3, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE,true);
		vbo->setElementDescription(0, YVbo::Element(3)); //Positions
		vbo->setElementDescription(1, YVbo::Element(3)); //Normales
		vboIndex->setElementDescription(0, YVbo::Element(1)); //Index Position
		vboIndex->setElementDescription(1, YVbo::Element(1)); //Index normale
		vbo->createVboCpu();
		vboIndex->createVboCpu();

		while (std::getline(file, line))
		{
			string pref = line.substr(0, 2);
			if (pref == "v " || pref == "vn" || pref == "f ")
			{
				Interpret(line);
			}
		}

		vbo->createVboGpu();
		vboIndex->createVboGpu();

		vboIndex->deleteVboCpu();
		vbo->deleteVboCpu();
	}
	file.close();
	return false;
}

void ObjImporter::Interpret(string &value)
{
	std::vector<string> values;
	splitString(values, value, ' ');

	//Vertice
	if (values[0] == "v")
	{
		float value1 = std::stod(values[1]);
		float value2 = std::stod(values[2]);
		float value3 = std::stod(values[3]);
		vbo->setElementValue(0, countVertices, value1, value2, value3);
		countVertices++;
	}

	if (values[0] == "vn")
	{
		float value1 = std::stod(values[1]);
		float value2 = std::stod(values[2]);
		float value3 = std::stod(values[3]);
		vbo->setElementValue(1, countNormal, value1, value2, value3);
		countNormal++;
	}

	if (values[0] == "f")
	{
		std::vector<string> splittedSlash;
		float value2;
		float value1;
		for (std::vector<string>::iterator it = values.begin() + 1; it != values.end(); it++)
		{
			countIndex++;
			splitString(splittedSlash, (*it), '/');
			value1 = std::stod(splittedSlash[0]);
			value2 = std::stod(splittedSlash[1]);
			vboIndex->setElementValue(0, countIndex, value1);
			vboIndex->setElementValue(1, countIndex, value2);
			splittedSlash.clear();
		}
	}
	values.clear();
}

