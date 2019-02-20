#include <conio.h>
#include <vector>
#include <string>
#include <Windows.h>

#include "engine_synthwave.h"

int main(int argc, char* argv[])
{
	YEngine * engine = SynthEngine::getInstance();

	engine->initBase(argc, argv);

	glutMainLoop();

	return 0;
}