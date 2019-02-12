#ifndef __WORLD_H__
#define __WORLD_H__

#include "external/gl/glew.h"
#include "external/gl/freeglut.h" 
#include "engine/utils/types_3d.h"
#include "cube.h"
#include "chunk.h"
#include "engine\noise\perlin.h"
#include "engine/render/TexHolder.h"

class MWorld
{
public :
	YPerlin perl;
	typedef uint8 MAxis;
	unsigned int textIndex;
	static const int AXIS_X = 0b00000001;
	static const int AXIS_Y = 0b00000010;
	static const int AXIS_Z = 0b00000100;

	#ifdef _DEBUG
	static const int MAT_SIZE = 3; //en nombre de chunks
	#else
	static const int MAT_SIZE = 3; //en nombre de chunks
	#endif // DEBUG

	static const int MAT_HEIGHT = 1; //en nombre de chunks
	static const int MAT_SIZE_CUBES = (MAT_SIZE * MChunk::CHUNK_SIZE);
	static const int MAT_HEIGHT_CUBES = (MAT_HEIGHT * MChunk::CHUNK_SIZE);
	static const int MAT_SIZE_METERS = (MAT_SIZE * MChunk::CHUNK_SIZE * MCube::CUBE_SIZE);
	static const int MAT_HEIGHT_METERS = (MAT_HEIGHT * MChunk::CHUNK_SIZE  * MCube::CUBE_SIZE);

	MChunk * Chunks[MAT_SIZE][MAT_SIZE][MAT_HEIGHT];
	YVec3f _Gravity;
	int heightTex;
	int widthTex;
	
	MWorld()
	{
		string nameTex("textures/atlas.jpg");
		TexHolder::GetInstance()->AddTexture(nameTex);
		textIndex = TexHolder::GetInstance()->GetTexture(nameTex);

		_Gravity = YVec3f(0, 0, -7.5f);
		//On crée les chunks
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for (int z = 0; z < MAT_HEIGHT; z++)
					Chunks[x][y][z] = new MChunk(x, y, z);				

		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
				{
					MChunk * cxPrev = NULL;
					if(x > 0)
						cxPrev = Chunks[x-1][y][z];
					MChunk * cxNext = NULL;
					if(x < MAT_SIZE-1)
						cxNext = Chunks[x+1][y][z];

					MChunk * cyPrev = NULL;
					if(y > 0)
						cyPrev = Chunks[x][y-1][z];
					MChunk * cyNext = NULL;
					if(y < MAT_SIZE-1)
						cyNext = Chunks[x][y+1][z];

					MChunk * czPrev = NULL;
					if(z > 0)
						czPrev = Chunks[x][y][z-1];
					MChunk * czNext = NULL;
					if(z < MAT_HEIGHT-1)
						czNext = Chunks[x][y][z+1];

					Chunks[x][y][z]->setVoisins(cxPrev,cxNext,cyPrev,cyNext,czPrev,czNext);
				}					
	}

	inline MCube * getCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * MChunk::CHUNK_SIZE) x = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * MChunk::CHUNK_SIZE) y = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(z >= MAT_HEIGHT * MChunk::CHUNK_SIZE) z = (MAT_HEIGHT * MChunk::CHUNK_SIZE)-1;

		return &(Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->_Cubes[x % MChunk::CHUNK_SIZE][y % MChunk::CHUNK_SIZE][z % MChunk::CHUNK_SIZE]);
	}

	void updateCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * MChunk::CHUNK_SIZE)x = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * MChunk::CHUNK_SIZE)y = (MAT_SIZE * MChunk::CHUNK_SIZE)-1;
		if (z >= MAT_HEIGHT * MChunk::CHUNK_SIZE)z = (MAT_HEIGHT * MChunk::CHUNK_SIZE) - 1; {
			Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->disableHiddenCubes();
			Chunks[x / MChunk::CHUNK_SIZE][y / MChunk::CHUNK_SIZE][z / MChunk::CHUNK_SIZE]->toVbos();
		}
		
	}

	void deleteCube(int x, int y, int z)
	{
		MCube * cube = getCube(x,y,z);
		cube->setType(MCube::CUBE_AIR);
		cube->setDraw(false);
		cube = getCube(x-1,y,z);
		updateCube(x,y,z);	
	}
			
	void init_world(int seed)
	{
		YLog::log(YLog::USER_INFO,(toString("Creation du monde seed ")+toString(seed)).c_str());

		srand(seed);
		
		//Reset du monde
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for (int z = 0; z < MAT_HEIGHT; z++)
				{
					Chunks[x][y][z]->reset();
					Chunks[x][y][z]->SetValues(seed);
					Chunks[x][y][z]->SetTexture(textIndex);
				}

		//Générer ici le monde en modifiant les cubes
		//Utiliser getCubes() 
		float value(rand());
		perl.setOffset(value / 1000.3f);

		MChunk * actualChunk;

		for (int z = 0; z <= MAT_SIZE_CUBES; z++)
		{
			for (int y = 0; y <= MAT_SIZE_CUBES; y++)
			{
				for (int x = 0; x <= MAT_SIZE_CUBES; x++)
				{
					int xValue(floor(x / MChunk::CHUNK_SIZE));
					int yValue(floor(y / MChunk::CHUNK_SIZE));
					int zValue(0);
					if (xValue * MChunk::CHUNK_SIZE == MAT_SIZE_CUBES) xValue--;
					if (yValue * MChunk::CHUNK_SIZE == MAT_SIZE_CUBES) yValue--;
					if (zValue * MChunk::CHUNK_SIZE == MAT_SIZE_CUBES) zValue--;

					actualChunk = Chunks[xValue][yValue][zValue];
					perl.setFreq(0.04f);
					float value = perl.sample((float)x, (float)y, (float)z);
					value = (value * 10);

					if (value + (z * actualChunk->_ScaleMountain) >= actualChunk->_MountainRadius && z > 0)
					{
						getCube(x, y, z)->setType(MCube::MCubeType::CUBE_AIR);
					}

					else
					{
						perl.setFreq(0.4f);
						value = perl.sample((float)x, (float)y, 0) * 10;
						if (abs(value - 5.0f) > (actualChunk->_GrassDensity * z))
						{
							getCube(x, y, z)->setType(MCube::MCubeType::CUBE_HERBE);

						}
						else
						{
							getCube(x, y, z)->setType(MCube::MCubeType::CUBE_TERRE);
						}

						//Evite les cubes flottants
						if (z > 0 && !getCube(x, y, z - 1)->isSolid())
						{
							getCube(x, y, z)->setType(MCube::MCubeType::CUBE_AIR);
						}
					}
				}
			}
		}

		for (int z = 0; z <= MAT_SIZE_CUBES; z++)
		{
			for (int y = 0; y <= MAT_SIZE_CUBES; y++)
			{
				for (int x = 0; x <= MAT_SIZE_CUBES; x++)
				{
					//Permet de creer des plans d'eaux
					if (getCube(x, y, z)->isSolid() && !getCube(x, y, z + 1)->isSolid() && !getCube(x + 1, y, z + 1)->isSolid() && !getCube(x - 1, y, z + 1)->isSolid() &&
						!getCube(x, y + 1, z + 1)->isSolid() && !getCube(x, y - 1, z + 1)->isSolid() && getCube(x, y - 1, z)->isAcceptableForWater() && getCube(x, y + 1, z)->isAcceptableForWater() &&
						getCube(x + 1, y, z)->isAcceptableForWater() && getCube(x - 1, y, z)->isAcceptableForWater())
					{
						value = perl.sample((float)x, (float)y, (float)z) * 10.0f;
						if (value + (z * actualChunk->_WaterDensity) <= actualChunk->_BaseChanceWater)
							getCube(x, y, z)->setType(MCube::MCubeType::CUBE_EAU);
					}
				}
			}
		}

		add_world_to_vbo();
	}

	void add_world_to_vbo(void)
	{
		for (int x = 0; x<MAT_SIZE; x++)
			for (int y = 0; y<MAT_SIZE; y++)
				for (int z = 0; z<MAT_HEIGHT; z++)
				{
					Chunks[x][y][z]->toVbos();
				}
	}
	
	void RemoveCube(YVec3f direction, YVec3f Position)
	{
		/*for (int i = 1; i < 5; i++)
		{
			YVec3f target = Position + (direction.normalize() * MCube::CUBE_SIZE * i);
			MCube * cb = getCube((int)floor(target.X), (int)floor(target.Y), (int)floor(target.Z));
			if (cb->isSolid())
			{
				cb->setType(MCube::MCubeType::CUBE_AIR);
				updateCube((int)floor(target.X), (int)floor(target.Y), (int)floor(target.Z));
				break;
			}
		}*/

		int RoundedX;
		int RoundedY;
		int RoundedZ;
		int x;
		int y;
		int z;

		int finalZ(0);
		int finalY(0);
		int finalX(0);

		bool found(false);
		YVec3f * pos = NULL;

		RoundedX = (int)floor(Position.X);
		RoundedY = (int)floor(Position.Y);
		RoundedZ = (int)floor(Position.Z);

		//Parcourir les blocs environnants
		for (x = 0; x >= -4; x--)
			{	
				if (found)
					break;
			for (y = 0; y >= -4; y--)
			{
				if (found)
					break;

				for (z = 0; z >= -4; z--)
				{
					if (found)
						break;

					if (RoundedX + x >= 0 && RoundedX + x < MAT_SIZE_CUBES && RoundedY + y >= 0 && RoundedY + y < MAT_SIZE_CUBES && RoundedZ + z >= 0 && RoundedZ + z < MAT_SIZE_CUBES && getCube(x + RoundedX, y + RoundedY, z + RoundedZ)->isSolid())
					{
						YVec3f origin(YVec3f(RoundedX + x, RoundedY + y, RoundedZ + z) * MCube::CUBE_SIZE);
						YVec3f Xp(origin + YVec3f(MCube::CUBE_SIZE, 0, 0));
						YVec3f Yp(origin + YVec3f(0, MCube::CUBE_SIZE, 0));
						YVec3f Zp(origin + YVec3f(0, 0, MCube::CUBE_SIZE));
						YVec3f XpYp(origin + YVec3f(MCube::CUBE_SIZE, MCube::CUBE_SIZE, 0));
						YVec3f XpZp(origin + YVec3f(MCube::CUBE_SIZE, 0, MCube::CUBE_SIZE));
						YVec3f YpZp(origin + YVec3f(0, MCube::CUBE_SIZE, MCube::CUBE_SIZE));
						YVec3f XpYpZp(origin + YVec3f(MCube::CUBE_SIZE, MCube::CUBE_SIZE, MCube::CUBE_SIZE));

						//Plan X / Y
						pos = intersecDroitePlan(origin, Xp, Yp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Yp, Xp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Xp, Yp, XpYp))
							found = true;

						pos = intersecDroitePlan(Zp, XpZp, YpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Zp, YpZp, XpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, XpYpZp, XpZp, YpZp))
							found = true;

						//Plan X / Z
						pos = intersecDroitePlan(origin, Xp, XpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Xp, XpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, origin, XpZp, Zp))
							found = true;

						pos = intersecDroitePlan(Yp, XpYp, XpYpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Yp, XpYpZp, XpYp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Yp, YpZp, XpYpZp))
							found = true;

						//Plan Y / Z
						pos = intersecDroitePlan(origin, Yp, YpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Yp, YpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, origin, Zp, YpZp))
							found = true;

						pos = intersecDroitePlan(Xp, XpYp, XpYpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Xp, XpYp, XpYpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Xp, XpYpZp, XpZp))
							found = true;

						if (found)
						{
							finalX = x;
							finalY = y;
							finalZ = z;
						}
					}
				}
			}
		}

		//Parcourir les blocs environnants
		for (x = 1; x <= 4; x++)
		{
			if (found)
				break;
			for (y = 1; y <= 4; y++)
			{
				if (found)
					break;

				for (z = 1; z <= 4; z++)
				{
					if (found)
						break;

					if (RoundedX + x >= 0 && RoundedX + x < MAT_SIZE_CUBES && RoundedY + y >= 0 && RoundedY + y < MAT_SIZE_CUBES && RoundedZ + z >= 0 && RoundedZ + z < MAT_SIZE_CUBES && getCube(x + RoundedX, y + RoundedY, z + RoundedZ)->isSolid())
					{
						YVec3f origin(YVec3f(RoundedX + x, RoundedY + y, RoundedZ + z) * MCube::CUBE_SIZE);
						YVec3f Xp(origin + YVec3f(MCube::CUBE_SIZE, 0, 0));
						YVec3f Yp(origin + YVec3f(0, MCube::CUBE_SIZE, 0));
						YVec3f Zp(origin + YVec3f(0, 0, MCube::CUBE_SIZE));
						YVec3f XpYp(origin + YVec3f(MCube::CUBE_SIZE, MCube::CUBE_SIZE, 0));
						YVec3f XpZp(origin + YVec3f(MCube::CUBE_SIZE, 0, MCube::CUBE_SIZE));
						YVec3f YpZp(origin + YVec3f(0, MCube::CUBE_SIZE, MCube::CUBE_SIZE));
						YVec3f XpYpZp(origin + YVec3f(MCube::CUBE_SIZE, MCube::CUBE_SIZE, MCube::CUBE_SIZE));

						//Plan X / Y
						pos = intersecDroitePlan(origin, Xp, Yp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Yp, Xp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Xp, Yp, XpYp))
							found = true;

						pos = intersecDroitePlan(Zp, XpZp, YpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Zp, YpZp, XpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, XpYpZp, XpZp, YpZp))
							found = true;

						//Plan X / Z
						pos = intersecDroitePlan(origin, Xp, XpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Xp, XpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, origin, XpZp, Zp))
							found = true;

						pos = intersecDroitePlan(Yp, XpYp, XpYpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Yp, XpYpZp, XpYp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Yp, YpZp, XpYpZp))
							found = true;

						//Plan Y / Z
						pos = intersecDroitePlan(origin, Yp, YpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, origin, Yp, YpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, origin, Zp, YpZp))
							found = true;

						pos = intersecDroitePlan(Xp, XpYp, XpYpZp, direction, Position);
						if (pos && intersecDroiteCubeFace(*pos, Xp, XpYp, XpYpZp))
							found = true;
						if (pos && intersecDroiteCubeFace(*pos, Xp, XpYpZp, XpZp))
							found = true;

						if (found)
						{
							finalX = x;
							finalY = y;
							finalZ = z;
						}
					}
				}
			}
		}

		if (pos)
		{
			MCube * cb = getCube(finalX + RoundedX, finalY + RoundedY, finalZ + RoundedZ);
			cb->setType(MCube::CUBE_AIR);
			updateCube(x + RoundedX, y + RoundedY, z + RoundedZ);
		}
	}
	
	//Boites de collisions plus petites que deux cubes
	MAxis getMinCol(YVec3f pos, YVec3f dir, float width, float height, float & valueColMin, bool oneShot, MWorld::MAxis mask = 0b00000111)
	{
		int x = (int)(pos.X / MCube::CUBE_SIZE);
		int y = (int)(pos.Y / MCube::CUBE_SIZE);
		int z = (int)(pos.Z / MCube::CUBE_SIZE);
		int xNext = (int)((pos.X + width / 2.0f) / MCube::CUBE_SIZE);
		int yNext = (int)((pos.Y + width / 2.0f) / MCube::CUBE_SIZE);
		int zNext = (int)((pos.Z + height / 2.0f) / MCube::CUBE_SIZE);

		int xPrev = (int)((pos.X - width / 2.0f) / MCube::CUBE_SIZE);
		int yPrev = (int)((pos.Y - width / 2.0f) / MCube::CUBE_SIZE);
		int zPrev = (int)((pos.Z - height / 2.0f) / MCube::CUBE_SIZE);

		if (x < 0)	x = 0;
		if (y < 0)	y = 0;
		if (z < 0)	z = 0;

		if (xPrev < 0)	xPrev = 0;
		if (yPrev < 0)	yPrev = 0;
		if (zPrev < 0)	zPrev = 0;

		if (xNext < 0)	xNext = 0;
		if (yNext < 0)	yNext = 0;
		if (zNext < 0)	zNext = 0;

		if (x >= MAT_SIZE_CUBES)	x = MAT_SIZE_CUBES - 1;
		if (y >= MAT_SIZE_CUBES)	y = MAT_SIZE_CUBES - 1;
		if (z >= MAT_HEIGHT_CUBES)	z = MAT_HEIGHT_CUBES - 1;

		if (xPrev >= MAT_SIZE_CUBES)	xPrev = MAT_SIZE_CUBES - 1;
		if (yPrev >= MAT_SIZE_CUBES)	yPrev = MAT_SIZE_CUBES - 1;
		if (zPrev >= MAT_HEIGHT_CUBES)	zPrev = MAT_HEIGHT_CUBES - 1;

		if (xNext >= MAT_SIZE_CUBES)	xNext = MAT_SIZE_CUBES - 1;
		if (yNext >= MAT_SIZE_CUBES)	yNext = MAT_SIZE_CUBES - 1;
		if (zNext >= MAT_HEIGHT_CUBES)	zNext = MAT_HEIGHT_CUBES - 1;

		//On fait chaque axe
		MAxis axis = 0x00;
		valueColMin = oneShot ? 0.5f : 10000.0f;
		float seuil = 0.0000001f;
		float prodScalMin = 1.0f;
		if (dir.getSqrSize() > 1)
			dir.normalize();

		//On verif tout les 4 angles de gauche
		if ((mask & AXIS_X) && (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev + 1, yPrev, zPrev)->isSolid() ||
				getCube(xPrev + 1, yPrev, zNext)->isSolid() ||
				getCube(xPrev + 1, yNext, zPrev)->isSolid() ||
				getCube(xPrev + 1, yNext, zNext)->isSolid()) || !oneShot)
			{
				float depassement = ((xPrev + 1) * MCube::CUBE_SIZE) - (pos.X - width / 2.0f);
				float prodScal = abs(dir.X);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_X;
					}
			}
		}

		//float depassementx2 = (xNext * NYCube::CUBE_SIZE) - (pos.X + width / 2.0f);

		//On verif tout les 4 angles de droite
		if ((mask & AXIS_X) && (getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xNext - 1, yPrev, zPrev)->isSolid() ||
				getCube(xNext - 1, yPrev, zNext)->isSolid() ||
				getCube(xNext - 1, yNext, zPrev)->isSolid() ||
				getCube(xNext - 1, yNext, zNext)->isSolid()) || !oneShot)
			{
				float depassement = (xNext * MCube::CUBE_SIZE) - (pos.X + width / 2.0f);
				float prodScal = abs(dir.X);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_X;
					}
			}
		}

		//float depassementy1 = (yNext * NYCube::CUBE_SIZE) - (pos.Y + width / 2.0f);

		//On verif tout les 4 angles de devant
		if ((mask & AXIS_Y) && (getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yNext - 1, zPrev)->isSolid() ||
				getCube(xPrev, yNext - 1, zNext)->isSolid() ||
				getCube(xNext, yNext - 1, zPrev)->isSolid() ||
				getCube(xNext, yNext - 1, zNext)->isSolid()) || !oneShot)
			{
				float depassement = (yNext * MCube::CUBE_SIZE) - (pos.Y + width / 2.0f);
				float prodScal = abs(dir.Y);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Y;
					}
			}
		}

		//float depassementy2 = ((yPrev + 1) * NYCube::CUBE_SIZE) - (pos.Y - width / 2.0f);

		//On verif tout les 4 angles de derriere
		if ((mask & AXIS_Y) && (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev + 1, zPrev)->isSolid() ||
				getCube(xPrev, yPrev + 1, zNext)->isSolid() ||
				getCube(xNext, yPrev + 1, zPrev)->isSolid() ||
				getCube(xNext, yPrev + 1, zNext)->isSolid()) || !oneShot)
			{
				float depassement = ((yPrev + 1) * MCube::CUBE_SIZE) - (pos.Y - width / 2.0f);
				float prodScal = abs(dir.Y);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Y;
					}
			}
		}

		//On verif tout les 4 angles du haut
		if ((mask & AXIS_Z) && (getCube(xPrev, yPrev, zNext)->isSolid() ||
			getCube(xPrev, yNext, zNext)->isSolid() ||
			getCube(xNext, yPrev, zNext)->isSolid() ||
			getCube(xNext, yNext, zNext)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev, zNext - 1)->isSolid() ||
				getCube(xPrev, yNext, zNext - 1)->isSolid() ||
				getCube(xNext, yPrev, zNext - 1)->isSolid() ||
				getCube(xNext, yNext, zNext - 1)->isSolid()) || !oneShot)
			{
				float depassement = (zNext * MCube::CUBE_SIZE) - (pos.Z + height / 2.0f);
				float prodScal = abs(dir.Z);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Z;
					}
			}
		}

		//On verif tout les 4 angles du bas
		if ((mask & AXIS_Z) && (getCube(xPrev, yPrev, zPrev)->isSolid() ||
			getCube(xPrev, yNext, zPrev)->isSolid() ||
			getCube(xNext, yPrev, zPrev)->isSolid() ||
			getCube(xNext, yNext, zPrev)->isSolid()))
		{
			//On verif que resoudre cette collision est utile
			if (!(getCube(xPrev, yPrev, zPrev + 1)->isSolid() ||
				getCube(xPrev, yNext, zPrev + 1)->isSolid() ||
				getCube(xNext, yPrev, zPrev + 1)->isSolid() ||
				getCube(xNext, yNext, zPrev + 1)->isSolid()) || !oneShot)
			{
				float depassement = ((zPrev + 1) * MCube::CUBE_SIZE) - (pos.Z - height / 2.0f);
				float prodScal = abs(dir.Z);
				if (abs(depassement) > seuil)
					if (abs(depassement) < abs(valueColMin))
					{
						prodScalMin = prodScal;
						valueColMin = depassement;
						axis = AXIS_Z;
					}
			}
		}

		return axis;
	}
		
	void render_world_basic(GLuint shader, YVbo * vboCube) 
	{
		int prog;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
		int uniformLocation = glGetUniformLocation(prog, "cube_color");

		for (int x = 0; x < MAT_SIZE_CUBES; x++)
		{
			for (int y = 0; y < MAT_SIZE_CUBES; y++)
			{
				for (int z = 0; z < MAT_HEIGHT_CUBES; z++)
				{
					MCube * cb = getCube(x, y, z);
					if (cb->getType() == MCube::CUBE_TERRE || cb->getType() == MCube::CUBE_EAU || cb->getType() == MCube::CUBE_HERBE)
					{
						glPushMatrix();
						glTranslatef(x, y, z);
						YRenderer::getInstance()->updateMatricesFromOgl();
						YVec3f color = getColor(cb->getType());
						glUniform4f(uniformLocation, color.X, color.Y, color.Z, 1);
						YRenderer::getInstance()->sendMatricesToShader(prog);
						vboCube->render();
						glPopMatrix();
					}				
				}
			}
		}
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	YVec3f getColor(MCube::MCubeType type)
	{
		switch (type)
		{
			case MCube::CUBE_TERRE:return YVec3f(102.0f / 255.0f, 51.0f / 255.0f, 0.0f);
			case MCube::CUBE_EAU:return YVec3f(51.0f / 255.0f, 0.0f, 1.0f);
			case MCube::CUBE_HERBE:return YVec3f(0.0f, 1.0f, 0.0f);
			default: return YVec3f(0.0f, 0.0f, 0.0f);
		}
	}

	void render_world_vbo(bool debug,bool doTransparent)
	{
		glDisable(GL_BLEND);
		//Dessiner les chunks opaques
		int prog;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
		for (int x = 0; x < MAT_SIZE; x++)
			for (int y = 0; y < MAT_SIZE; y++)
				for (int z = 0; z < MAT_HEIGHT; z++)
				{
					YRenderer::getInstance()->updateMatricesFromOgl();
					YRenderer::getInstance()->sendMatricesToShader(prog);
					Chunks[x][y][z]->render(0);
					Chunks[x][y][z]->render(1);
				}
					

		if (!doTransparent)
			return;
		glEnable(GL_BLEND);
		//Dessiner les chunks transparents
	}

	/**
	* Attention ce code n'est pas optimal, il est compréhensible. Il existe de nombreuses
	* versions optimisées de ce calcul.
	*/
	YVec3f * intersecDroitePlan(YVec3f plan1 , YVec3f plan2 , YVec3f plan3, YVec3f direction , YVec3f positionDroite)
	{
		YVec3f * output = NULL;
		YVec3f normal = (plan2 - plan1).cross(plan3 - plan1).normalize();
		direction = direction.normalize();

		float a(normal.X);
		float b(normal.Y);
		float c(normal.Z);
		float d((normal.X * plan1.X) - (normal.Y * plan1.Y) - (normal.Z * plan1.Z));

		float t(0);
		float x((direction.X * t) + positionDroite.X);
		float y((direction.Y * t) + positionDroite.Y);
		float z((direction.Z * t) + positionDroite.Z);
		float denom((a* direction.X) + (b * direction.Y) + (c * direction.Z));
		if (denom != 0.0f)
		{
			t = -((a * positionDroite.X) + (b * positionDroite.Y) + (c * positionDroite.Z) + d) / denom;
			output = new YVec3f((direction.X * t) + positionDroite.X, (direction.Y * t) + positionDroite.Y, (direction.Z * t) + positionDroite.Z);
		}
		return output;
	}

	/**
	* Attention ce code n'est pas optimal, il est compréhensible. Il existe de nombreuses
	* versions optimisées de ce calcul. Il faut donner les points dans l'ordre (CW ou CCW)
	*/
	inline bool intersecDroiteCubeFace(YVec3f point , YVec3f vertice1 , YVec3f vertice2 , YVec3f vertice3)
	{
		YVec3f ANormal((vertice2 - vertice1).cross(point - vertice1));
		YVec3f BNormal((vertice3 - vertice2).cross(point - vertice2));
		YVec3f CNormal((vertice1 - vertice3).cross(point - vertice3));
		
		float angleA = ANormal.dot(BNormal);
		float angleB = BNormal.dot(CNormal);
		float angleC = CNormal.dot(ANormal);

		bool ok(angleA > 0 && angleB > 0 && angleC > 0);

		return ok;
	}

	bool getRayCollision(const YVec3f & debSegment, const YVec3f & finSegment,
		YVec3f & inter,
		int &xCube, int&yCube, int&zCube)
	{
		
		return false;
	}

	/**
	* De meme cette fonction peut être grandement opitimisée, on a priviligié la clarté
	*/
	bool getRayCollisionWithCube(const YVec3f & debSegment, const YVec3f & finSegment,
		int x, int y, int z,
		YVec3f & inter)
	{

		return true;
	}
};



#endif