#ifndef SYNTH_ENGINE
#define SYNTH_ENGINE

#include "engine/engine.h"

class SynthEngine : public YEngine
{
	public:
		YVbo * Floor;
		static YEngine * getInstance()
		{
			if (Instance == NULL)
				Instance = new SynthEngine();
			return Instance;
		}

		void init()
		{
		}

		void update(float elapsed)
		{

		}

		void renderObjects()
		{

		}

		void resize(int width, int height)
		{

		}

		void keyPressed(int key, bool special, bool down, int p1, int p2)
		{

		}

		void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
		{

		}

		void mouseClick(int button, int state, int x, int y, bool inUi)
		{

		}

		void mouseMove(int x, int y, bool pressed, bool inUi)
		{

		}
};

#endif
