#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "VboIndex.h"
#include <string>
#include <map>
#include <fstream>

class ObjImporter
{
	private:
		YVboIndex * vbo;
		string nameFile;
		int countVertices;
		int countIndex;
		int countNormal;
		int nbVertices;
		int nbIndex;
		std::vector<unsigned int> indicesNormals;
		std::vector<unsigned int> indicesPositions;
		std::vector<YVec3f> listPosition;
		std::vector<YVec3f> listNormales;

	public:

		ObjImporter(string fileName);
		bool Initialize();

		inline void render()
		{
			vbo->render();
		}

		YVboIndex * getVbo()
		{
			return vbo;
		}

		map<YVec3f, YVec3f, YVec3<float>::VecCompare> getSumNormal();
		void setNormals(map<YVec3f, YVec3f, YVec3<float>::VecCompare> input);
};

#endif OBJ_IMPORTER_H
