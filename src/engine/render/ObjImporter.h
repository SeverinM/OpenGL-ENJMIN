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
		void Interpret(string &value);
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
};

#endif OBJ_IMPORTER_H
