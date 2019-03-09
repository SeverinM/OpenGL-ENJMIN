#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "VboIndex.h"
#include <string>

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
};

#endif OBJ_IMPORTER_H
