#pragma once

#include "engine/render/vbo.h"
#include "engine/noise/perlin.h"

class Decor
{
	private :
		YVbo * ground;
		float sizeGround;
		YVec3f originGround;

		YVbo * mountains;
		float * sizeMountains;
		YVec3f originMountains;

	public:

		YVbo * getGround()
		{
			return ground;
		}

		YVbo * getMountains()
		{
			return mountains;
		}

		YVec3f getoriginGround()
		{
			return originGround;
		}

		YVec3f getoriginMountains()
		{
			return originMountains;
		}

		void GenerateGround(int lenght, int width, float groundSize, YVec3f origin = YVec3f(0,0,0))
		{
			originGround = origin;
			ground = new YVbo(3, (lenght * width * 6), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, false);
			ground->setElementDescription(0, YVbo::Element(3)); //Position
			ground->setElementDescription(1, YVbo::Element(4)); //Color border
			ground->setElementDescription(2, YVbo::Element(4)); //Color filled
			ground->createVboCpu();

			int indexPosition(0);
			int indexColor(0);

			for (int x = 0; x < lenght; x++)
			{
				for (int y = 0; y < width; y++)
				{
					ground->SetFace(YVec3f(x * groundSize, y * groundSize, 0), YVec3f(1, 0, 0), YVec3f(0, 1, 0), groundSize, indexPosition, true);
					for (int i = 0; i < 6; i++)
					{
						ground->setElementValue(1, indexColor, 1, 0, 1, 1);
						ground->setElementValue(2, indexColor, 0, 0, 35.0 / 255.0, 1);
						indexColor++;
					}
				}
			}

			ground->createVboGpu();
			ground->deleteVboCpu();
		}

		void GenerateMountains(int lenght, int width, float noise, float size, YVec3f origin)
		{
			YPerlin perl;
			float frequence(noise);

			originMountains = origin;
			mountains = new YVbo(3, (lenght * width * 6), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, false);
			mountains->setElementDescription(0, YVbo::Element(3)); //Position
			mountains->setElementDescription(1, YVbo::Element(4)); //Color border
			mountains->setElementDescription(2, YVbo::Element(4)); //Color filled
			mountains->createVboCpu();

			int indexPosition(0);
			int indexColor(0);
			float random;
			float randomXp;
			float randomYp;
			float randomXpYp;

			for (int x = 0; x < lenght; x++)
			{
				for (int y = 0; y < width; y++)
				{
					random = max(0,(perl.sample(x * frequence, y * frequence, 0) * 50) - 20);
					randomXp = max(0, (perl.sample((x + 1) * frequence, y * frequence, 0) * 50) - 20);
					randomYp = max(0, (perl.sample(x * frequence, (y + 1) * frequence, 0) * 50) - 20);
					randomXpYp = max(0, (perl.sample((x + 1) * frequence, (y + 1) * frequence, 0) * 50) - 20);

					//Avant derniere colonne
					if (x == lenght - 2)
					{
						randomXp = randomXpYp = 0;
					}

					//Premiere colonne
					if (x == 0)
					{
						random = randomYp = 0;
					}

					//Derniere colonne
					if (x == lenght - 1 || y == width - 1)
					{
						randomXp = randomXpYp = randomYp = random = 0;
					}

					//Avant derniere ligne
					if (y == lenght - 2)
					{
						randomYp = randomXpYp = 0;
					}

					if (y == 0)
					{
						random = randomXp = 0;
					}

					mountains->setElementValue(0, indexPosition, x * size, y * size, random);
					mountains->setElementValue(0, indexPosition + 1, (x + 1) * size, y * size, randomXp);
					mountains->setElementValue(0, indexPosition + 2, x * size, (y + 1) * size, randomYp);
					mountains->setElementValue(0, indexPosition + 3, (x + 1) * size, (y + 1) * size, randomXpYp);
					mountains->setElementValue(0, indexPosition + 4, x * size, (y + 1) * size, randomYp);
					mountains->setElementValue(0, indexPosition + 5, (x + 1) * size, y * size, randomXp);

					indexPosition += 6;

					for (int i = 0; i < 6; i++)
					{
						mountains->setElementValue(1, indexColor, 0, 1, 1, 1);
						mountains->setElementValue(2, indexColor, 0, 0, 35.0 / 255.0, 1);
						indexColor++;
					}
				}
			}

			mountains->createVboGpu();
			mountains->deleteVboCpu();
		}
};