#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "vbo.h"
#include <string>

class ObjImporter
{
	private:
		YVbo * vbo;
		YVbo * vboIndex;
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

		YVbo * getVbo()
		{
			return vbo;
		}
};

#endif OBJ_IMPORTER_H
