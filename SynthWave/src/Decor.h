#pragma once

#include "engine/render/vbo.h"

class Decor
{
	private :
		YVbo * ground;
		float sizeGround;
		YVec3f offset;

	public:
		Decor()
		{
			offset = YVec3f(1, 1, 0.1);
		}

		YVbo * getGround()
		{
			return ground;
		}

		YVec3f getOffset()
		{
			return offset;
		}

		void GenerateGround(int lenght, int width, float groundSize)
		{
			ground = new YVbo(3, lenght * width * 6, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE, false);
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
					YVec3 prod(YVec3f(1, 0, 0).cross(YVec3f(0, 1, 0)));

					for (int i = 0; i < 6; i++)
					{
						ground->setElementValue(1, indexColor, 1, 0, 1, 1);
						ground->setElementValue(2, indexColor, 0, 0, 15.0 / 255.0, 1);
						indexColor++;
					}
				}
			}

			ground->createVboGpu();
			ground->deleteVboCpu();
		}
};