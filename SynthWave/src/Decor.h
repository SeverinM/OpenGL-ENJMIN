#pragma once

#include "engine/render/vbo.h"
#include "engine/noise/perlin.h"
#include "../SkyBox.h"

class Decor
{
	private :
		YVbo * ground;
		float sizeGround;
		YVec3f originGround;

		YVbo * sun;

		float * sizeMountains;

		std::vector<std::pair<YVec3f,YVbo *>> Mountains;

		YPerlin * perl;


	public:

		Decor(float seed)
		{
			seed *= 0.000000001f;
			perl = new YPerlin();
			perl->setOffset(seed);
		}

		YVbo * getGround()
		{
			return ground;
		}

		std::vector<std::pair<YVec3f, YVbo *>> getMountains()
		{
			return Mountains;
		}

		YVbo * getSun()
		{
			return sun;
		}

		YVec3f getoriginGround()
		{
			return originGround;
		}

		void GenerateGround(int lenght, int width, float groundSize,GLuint textureBorder, YVec3f origin = YVec3f(0,0,0))
		{
			originGround = origin;
			ground = new YVbo(4, (lenght * width * 6), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, false);
			ground->SetTexture(textureBorder);
			ground->setElementDescription(0, YVbo::Element(3)); //Position
			ground->setElementDescription(1, YVbo::Element(4)); //Color border
			ground->setElementDescription(2, YVbo::Element(4)); //Color filled
			ground->setElementDescription(3, YVbo::Element(2)); //UV
			ground->createVboCpu();

			int indexPosition(0);
			int indexColor(0);
			int indexUV(0);

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

					for (int j = 0; j < 2; j++)
					{
						ground->setElementValue(3, indexUV, 0, 0);
						indexUV++;
						ground->setElementValue(3, indexUV, 1, 0);
						indexUV++;
						ground->setElementValue(3, indexUV, 0, 1);
						indexUV++;
					}
				}
			}

			ground->createVboGpu();
			ground->deleteVboCpu();
		}

		void GenerateSun(SkyBox * sb)
		{
			sun = new YVbo(1, 36, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
			sun->setElementDescription(0, YVbo::Element(3)); //Position
			sun->createVboCpu();

			int indexPos(0);

			//Face X / Y
			sun->SetFace(YVec3f(-1, -1, -1), YVec3f(1, 0, 0), YVec3f(0, 1, 0), 2, indexPos, true);
			sun->SetFace(YVec3f(-1, -1, 1), YVec3f(1, 0, 0), YVec3f(0, 1, 0), 2, indexPos, true);

			//Face Y / Z
			sun->SetFace(YVec3f(-1, -1, -1), YVec3f(0, 1, 0), YVec3f(0, 0, 1), 2, indexPos, true);
			sun->SetFace(YVec3f(1, -1, -1), YVec3f(0, 1, 0), YVec3f(0, 0, 1), 2, indexPos, true);

			//Face X / Z
			sun->SetFace(YVec3f(-1, -1, -1), YVec3f(1, 0, 0), YVec3f(0, 0, 1), 2, indexPos, true);
			sun->SetFace(YVec3f(-1, 1, -1), YVec3f(1, 0, 0), YVec3f(0, 0, 1), 2, indexPos, true);

			sun->createVboGpu();
			sun->deleteVboCpu();

			sun->SetTextureCube(sb->GetCube());
		}

		void GenerateMountains(int lenght, int width, float noise, float size, YVec3f origin, GLuint texture)
		{
			YVbo * mountains;
			std::pair<YVec3f, YVbo *> data;
			data.first = origin;

			float frequence(noise);
			mountains = new YVbo(4, (lenght * width * 6), YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, false);
			mountains->setElementDescription(0, YVbo::Element(3)); //Position
			mountains->setElementDescription(1, YVbo::Element(4)); //Color border
			mountains->setElementDescription(2, YVbo::Element(4)); //Color filled
			mountains->setElementDescription(3, YVbo::Element(2)); //UV
			mountains->SetTexture(texture);
			mountains->createVboCpu();

			int indexPosition(0);
			int indexColor(0);
			int indexUV(0);

			float random;
			float randomXp;
			float randomYp;
			float randomXpYp;

			for (int x = 0; x < lenght; x++)
			{
				for (int y = 0; y < width; y++)
				{
					random = max(0,(perl->sample(x * frequence, y * frequence, 0) * 50) - 20);
					randomXp = max(0, (perl->sample((x + 1) * frequence, y * frequence, 0) * 50) - 20);
					randomYp = max(0, (perl->sample(x * frequence, (y + 1) * frequence, 0) * 50) - 20);
					randomXpYp = max(0, (perl->sample((x + 1) * frequence, (y + 1) * frequence, 0) * 50) - 20);

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

					for (int j = 0; j < 2; j++)
					{
						mountains->setElementValue(3, indexUV, 0, 0);
						indexUV++;
						mountains->setElementValue(3, indexUV, 1, 0);
						indexUV++;
						mountains->setElementValue(3, indexUV, 0, 1);
						indexUV++;
					}
				}
			}

			mountains->createVboGpu();
			mountains->deleteVboCpu();
			data.second = mountains;
			Mountains.push_back(data);
		}
};