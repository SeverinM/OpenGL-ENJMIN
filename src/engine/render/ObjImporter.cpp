#pragma once
#include <vector>
#include "ObjImporter.h"

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

	ifstream infile(nameFile);
	string line;
	vector<string> splitted;
	while (std::getline(infile, line))
	{
		splitString(splitted, line, ' ');
		
		if (splitted[0] == "v")
		{
			YVec3f temp(std::stof(splitted[1]), std::stof(splitted[2]), std::stof(splitted[3]));
			listPosition.push_back(temp);
		}

		if (splitted[0] == "vn")
		{
			YVec3f temp(std::stof(splitted[1]), std::stof(splitted[2]), std::stof(splitted[3]));
			listNormales.push_back(temp);
		}

		if (splitted[0] == "f")
		{
			std::vector<string> subSplit1;
			std::vector<string> subSplit2;
			std::vector<string> subSplit3;
			std::vector<string> subSplit4;

			splitString(subSplit1, splitted[1], '/');
			splitString(subSplit2, splitted[2], '/');
			splitString(subSplit3, splitted[3], '/');
			splitString(subSplit4, splitted[4], '/');

 			indicesPositions.push_back(std::stoi(subSplit1[0]));
			indicesPositions.push_back(std::stoi(subSplit2[0]));
			indicesPositions.push_back(std::stoi(subSplit3[0]));
			indicesPositions.push_back(std::stoi(subSplit4[0]));

			indicesNormals.push_back(std::stoi(subSplit1[1]));
			indicesNormals.push_back(std::stoi(subSplit2[1]));
			indicesNormals.push_back(std::stoi(subSplit3[1]));
			indicesNormals.push_back(std::stoi(subSplit4[1]));
		}

		splitted.clear();
	}

	getSumNormal();

	return true;
}

map<YVec3f, YVec3f, YVec3<float>::VecCompare> ObjImporter::getSumNormal()
{
	map<YVec3f, YVec3f, YVec3<float>::VecCompare> output;
	for (int indexPosition = 0; indexPosition < indicesPositions.size(); indexPosition++)
	{
		int indPos = indicesPositions[indexPosition];
		int indNorm = indicesNormals[indexPosition];
		YVec3f sampledPosition = listPosition[indPos - 1];
		YVec3f sampledNormal = listNormales[indNorm - 1];

		//La clé existe
		if (output.find(sampledPosition) != output.end())
		{
			output[sampledPosition] += sampledNormal.normalize();
		}
		else
		{
			output[sampledPosition] = sampledNormal.normalize();
		}
	}
	return output;
}

