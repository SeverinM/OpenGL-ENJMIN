#pragma once

#include "engine/render/renderer.h"
#include "engine/render/vbo.h"
#include "cube.h"

/**
  * On utilise des chunks pour que si on modifie juste un cube, on ait pas
  * besoin de recharger toute la carte dans le buffer, mais juste le chunk en question
  */
class MChunk
{
	private :
		unsigned int textureIndex;
	public :

		static const int CHUNK_SIZE = 32 ; ///< Taille d'un chunk en nombre de cubes (n*n*n)
		MCube _Cubes[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; ///< Cubes contenus dans le chunk

		YVbo * VboOpaque = NULL;
		YVbo * VboTransparent = NULL;
		MChunk * Voisins[6];

		int _XPos, _YPos, _ZPos; ///< Position du chunk dans le monde

		float _ScaleMountain; //Plus ce nombre est grand et moins les montagnes serons grandes
		float _MountainRadius; //Plus ce nombre est grand et plus les montagnes auront une petite taille
		float _GrassDensity; //Plus ce nombre est grand et plus l'herbe disparaitra rapidement
		float _WaterDensity; //Plus ce nombre est grand et moins il y a de chance de voir de l'eau en hauteur
		float _BaseChanceWater; //Proba d'avoir de l'eau dés la premiere couche, plus ce nombre est haut et plus la proba est elevé

		MChunk(int x, int y, int z)
		{
			memset(Voisins, 0x00, sizeof(void*)* 6);
			_XPos = x;
			_YPos = y;
			_ZPos = z;
			_ScaleMountain = 0.30f;
			_MountainRadius = 3.5f;
			_GrassDensity = 0.1f;
			_WaterDensity = 0.4f;
			_BaseChanceWater = 2.5f;
			textureIndex = 0;
		}

		//On fait varier la generation de terrain entre chunk
		void SetValues(int seed)
		{
			float scale(0.2f);
			float values(rand() % 10);
			_ScaleMountain += _ScaleMountain * ((values / 10.0f) - 0.5f) * 0.1f;
			values = rand() % 10;
			_MountainRadius += _MountainRadius * ((values / 10.0f) - 0.5f) * 0.1f;
			values = rand() % 10;
			_GrassDensity += _GrassDensity * ((values / 10.0f) - 0.5f) * 0.1f;
			values = rand() % 10;
			_WaterDensity += _WaterDensity * ((values / 10.0f) - 0.5f) * 0.1f;
			values = rand() % 10;
			_BaseChanceWater += _BaseChanceWater * ((values / 10.0f) - 0.5f) * 0.1f;
		}

		void SetTexture(unsigned int index)
		{
			textureIndex = index;
		}

		/*
		Creation des VBO
		*/

		//On met le chunk ddans son VBO
		void toVbos(void)
		{
			SAFEDELETE(VboOpaque);
			SAFEDELETE(VboTransparent);

			//Compter les sommets
			int countSomm(0);
			int countSommTrans(0);
			bool occlusion = false;
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					for (int z = 0; z < CHUNK_SIZE; z++)
					{
						if (!test_hidden(x,y,z))
						{
							if (_Cubes[x][y][z].isTransparent())
							{
								countSommTrans += 6;
							}
							else
							{
								if (x == 0 || !_Cubes[x - 1][y][z].isOpaque())
								{
									countSomm += 6;
								}

								if (x == CHUNK_SIZE -1 || !_Cubes[x + 1][y][z].isOpaque())
								{
									countSomm += 6;
								}

								if (y == 0 || !_Cubes[x ][y - 1][z].isOpaque())
								{
									countSomm += 6;
								}

								if (y == CHUNK_SIZE - 1 || !_Cubes[x][y + 1][z].isOpaque())
								{
									countSomm += 6;
								}

								if (z == 0 || !_Cubes[x][y][z - 1].isOpaque())
								{
									countSomm += 6;
								}

								if (z == CHUNK_SIZE - 1 || !_Cubes[x][y][z + 1].isOpaque())
								{
									countSomm += 6;
								}
							}
						}
					}
				}
			}


			//Créer les VBO
			VboOpaque = new YVbo(4, countSomm + 1, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
			VboOpaque->setElementDescription(0, YVbo::Element(3));//Position
			VboOpaque->setElementDescription(1, YVbo::Element(3));//Normale
			VboOpaque->setElementDescription(2, YVbo::Element(2));//UV
			VboOpaque->setElementDescription(3, YVbo::Element(1));//Type
			VboOpaque->createVboCpu();
			VboOpaque->SetTexture(textureIndex);

			VboTransparent = new YVbo(4, countSommTrans + 1, YVbo::DATA_STORAGE_METHOD::PACK_BY_VERTICE);
			VboTransparent->setElementDescription(0, YVbo::Element(3));//Position
			VboTransparent->setElementDescription(1, YVbo::Element(3));//Normale
			VboTransparent->setElementDescription(2, YVbo::Element(2));//UV
			VboTransparent->setElementDescription(3, YVbo::Element(1));//Type
			VboTransparent->createVboCpu();
			VboTransparent->SetTexture(textureIndex);
			
			countSommTrans = 0;
			countSomm = 0;

			int countTexture(0);
			int countNorm(0);
			int countType(0);

			int countTypeTransparent(0);
			int countTextureTransparent(0);
			int countNormTransparent(0);

			//Remplir les VBO
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					for (int z = 0; z < CHUNK_SIZE; z++)
					{
						if (!test_hidden(x,y,z))
						{
							glBindTexture(GL_TEXTURE_2D, textureIndex);
							int width, height;
							glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
							glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);


							if (_Cubes[x][y][z].isTransparent() && _Cubes[x][y][z].getType() != MCube::MCubeType::CUBE_AIR)
							{
								int type(_Cubes[x][y][z].getType());
								float XUnit;
								float YUnit;
								std::vector<std::pair<int, int>> value(MCube::GetUV(_Cubes[x][y][z].getType(), XUnit, YUnit));
								for (int i = 0; i < 6; i++)
								{
									VboTransparent->setElementValue(3, i + countTypeTransparent, type);
								}
								countTypeTransparent += 6;

								VboTransparent->SetFace(YVec3f(x * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, (z + 1) * MCube::CUBE_SIZE), YVec3f(1, 0, 0), YVec3f(0, 1, 0), MCube::CUBE_SIZE, countSommTrans, true);
								VboTransparent->SetNormale(YVec3f(1, 0, 0), YVec3f(0, 1, 0), countNormTransparent, 1);
								VboTransparent->SetTextureElt(countTextureTransparent, value, XUnit, YUnit,width, height, 2, 2);
							}
							if (_Cubes[x][y][z].isOpaque())
							{
								int type(_Cubes[x][y][z].getType());
								float XUnit;
								float YUnit;
								std::vector<std::pair<int, int>> value(MCube::GetUV(_Cubes[x][y][z].getType(), XUnit, YUnit));

								if (x == 0 || !_Cubes[x - 1][y][z].isOpaque())
								{
									VboOpaque->SetFace(YVec3f(x * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE), YVec3f(0, 1, 0), YVec3f(0, 0, 1), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(0, 1, 0), YVec3f(0, 0, 1), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width , height, 0, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}

								if (x == CHUNK_SIZE - 1 || !_Cubes[x + 1][y][z].isOpaque())
								{
									VboOpaque->SetFace(YVec3f((x + 1) * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE), YVec3f(0, 1, 0), YVec3f(0, 0, 1), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(0, 0, 1), YVec3f(0, 1, 0), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width, height, 0, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}

								if (y == 0 || !_Cubes[x][y - 1][z].isOpaque())
								{
									VboOpaque->SetFace(YVec3f(x * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE), YVec3f(1, 0, 0), YVec3f(0, 0, 1), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(1, 0, 0), YVec3f(0, 0, 1), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width, height, 1, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}

								if (y == CHUNK_SIZE - 1 || !_Cubes[x][y + 1][z].isOpaque())
								{
									VboOpaque->SetFace(YVec3f(x * MCube::CUBE_SIZE,(y + 1) * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE), YVec3f(1, 0, 0), YVec3f(0, 0, 1), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(0, 0, 1), YVec3f(1, 0, 0), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width, height, 1, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}

								if (z == 0 || !_Cubes[x][y][z - 1].isOpaque())
								{
									VboOpaque->SetFace(YVec3f(x * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, z * MCube::CUBE_SIZE), YVec3f(1, 0, 0), YVec3f(0, 1, 0), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(0, 1, 0), YVec3f(1, 0, 0), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width, height, 5, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}

								if (z == CHUNK_SIZE - 1 || !_Cubes[x][y][z + 1].isOpaque())
								{
									VboOpaque->SetFace(YVec3f(x * MCube::CUBE_SIZE, y * MCube::CUBE_SIZE, (z + 1) * MCube::CUBE_SIZE), YVec3f(1, 0, 0), YVec3f(0, 1, 0), MCube::CUBE_SIZE, countSomm, true);
									VboOpaque->SetNormale(YVec3f(1, 0, 0), YVec3f(0, 1, 0), countNorm, 1);
									VboOpaque->SetTextureElt(countTexture, value, XUnit, YUnit, width, height, 2, 2);
									for (int i = 0; i < 6; i++)
									{
										VboOpaque->setElementValue(3, i + countType, type);
									}
									countType += 6;
								}
							}

							glBindTexture(GL_TEXTURE_2D, 0);
						}
					}
				}
			}

			VboOpaque->createVboGpu();
			VboOpaque->deleteVboCpu();

			VboTransparent->createVboGpu();
			VboTransparent->deleteVboCpu();
		}

		//Ajoute un quad du cube. Attention CCW
		int addQuadToVbo(YVbo * vbo, int iVertice, YVec3f & a, YVec3f & b, YVec3f & c, YVec3f & d, float type) {

			return 6;
		}

		//Permet de compter les triangles ou des les ajouter aux VBO
		void foreachVisibleTriangle(bool countOnly, int * nbVertOpaque, int * nbVertTransp, YVbo * VboOpaque, YVbo * VboTrasparent) {


		}

		/*
		Gestion du chunk
		*/

		void reset(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z].setDraw(false);
						_Cubes[x][y][z].setType(MCube::CUBE_AIR);
					}
		}

		void setVoisins(MChunk * xprev, MChunk * xnext, MChunk * yprev, MChunk * ynext, MChunk * zprev, MChunk * znext)
		{
			Voisins[0] = xprev;
			Voisins[1] = xnext;
			Voisins[2] = yprev;
			Voisins[3] = ynext;
			Voisins[4] = zprev;
			Voisins[5] = znext;
		}

		void get_surrounding_cubes(int x, int y, int z, MCube ** cubeXPrev, MCube ** cubeXNext,
			MCube ** cubeYPrev, MCube ** cubeYNext,
			MCube ** cubeZPrev, MCube ** cubeZNext)
		{

			*cubeXPrev = NULL;
			*cubeXNext = NULL;
			*cubeYPrev = NULL;
			*cubeYNext = NULL;
			*cubeZPrev = NULL;
			*cubeZNext = NULL;

			if (x == 0 && Voisins[0] != NULL)
				*cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE - 1][y][z]);
			else if (x > 0)
				*cubeXPrev = &(_Cubes[x - 1][y][z]);

			if (x == CHUNK_SIZE - 1 && Voisins[1] != NULL)
				*cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
			else if (x < CHUNK_SIZE - 1)
				*cubeXNext = &(_Cubes[x + 1][y][z]);

			if (y == 0 && Voisins[2] != NULL)
				*cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE - 1][z]);
			else if (y > 0)
				*cubeYPrev = &(_Cubes[x][y - 1][z]);

			if (y == CHUNK_SIZE - 1 && Voisins[3] != NULL)
				*cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
			else if (y < CHUNK_SIZE - 1)
				*cubeYNext = &(_Cubes[x][y + 1][z]);

			if (z == 0 && Voisins[4] != NULL)
				*cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE - 1]);
			else if (z > 0)
				*cubeZPrev = &(_Cubes[x][y][z - 1]);

			if (z == CHUNK_SIZE - 1 && Voisins[5] != NULL)
				*cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
			else if (z < CHUNK_SIZE - 1)
				*cubeZNext = &(_Cubes[x][y][z + 1]);
		}

		void render(bool transparent)
		{
			if (transparent)
			{
				glEnable(GL_BLEND);
				glPushMatrix();
				glTranslatef(_XPos * CHUNK_SIZE * MCube::CUBE_SIZE, _YPos * CHUNK_SIZE * MCube::CUBE_SIZE, _ZPos * CHUNK_SIZE * MCube::CUBE_SIZE);
				YRenderer::getInstance()->updateMatricesFromOgl();
				YRenderer::getInstance()->sendMatricesToShader(YRenderer::CURRENT_SHADER);
				VboTransparent->render();
				glPopMatrix();
			}	
			else
			{
				glDisable(GL_BLEND);
				glPushMatrix();
				glTranslatef(_XPos * CHUNK_SIZE * MCube::CUBE_SIZE, _YPos * CHUNK_SIZE * MCube::CUBE_SIZE, _ZPos * CHUNK_SIZE * MCube::CUBE_SIZE);
				YRenderer::getInstance()->updateMatricesFromOgl();
				YRenderer::getInstance()->sendMatricesToShader(YRenderer::CURRENT_SHADER);
				VboOpaque->render();
				glPopMatrix();
			}
		}

		/**
		  * On verifie si le cube peut être vu
		  */
		bool test_hidden(int x, int y, int z)
		{
			MCube * cubeXPrev = NULL; 
			MCube * cubeXNext = NULL; 
			MCube * cubeYPrev = NULL; 
			MCube * cubeYNext = NULL; 
			MCube * cubeZPrev = NULL; 
			MCube * cubeZNext = NULL; 

			if(x == 0 && Voisins[0] != NULL)
				cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE-1][y][z]);
			else if(x > 0)
				cubeXPrev = &(_Cubes[x-1][y][z]);

			if(x == CHUNK_SIZE-1 && Voisins[1] != NULL)
				cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
			else if(x < CHUNK_SIZE-1)
				cubeXNext = &(_Cubes[x+1][y][z]);

			if(y == 0 && Voisins[2] != NULL)
				cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE-1][z]);
			else if(y > 0)
				cubeYPrev = &(_Cubes[x][y-1][z]);

			if(y == CHUNK_SIZE-1 && Voisins[3] != NULL)
				cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
			else if(y < CHUNK_SIZE-1)
				cubeYNext = &(_Cubes[x][y+1][z]);

			if(z == 0 && Voisins[4] != NULL)
				cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE-1]);
			else if(z > 0)
				cubeZPrev = &(_Cubes[x][y][z-1]);

			if(z == CHUNK_SIZE-1 && Voisins[5] != NULL)
				cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
			else if(z < CHUNK_SIZE-1)
				cubeZNext = &(_Cubes[x][y][z+1]);

			if( cubeXPrev == NULL || cubeXNext == NULL ||
				cubeYPrev == NULL || cubeYNext == NULL ||
				cubeZPrev == NULL || cubeZNext == NULL )
				return false;

			if (cubeXPrev->isOpaque() == true && //droite
				cubeXNext->isOpaque() == true && //gauche
				cubeYPrev->isOpaque() == true && //haut
				cubeYNext->isOpaque() == true && //bas
				cubeZPrev->isOpaque() == true && //devant
				cubeZNext->isOpaque() == true)  //derriere
				return true;
			return false;
		}

		void disableHiddenCubes(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z].setDraw(true);
						if(test_hidden(x,y,z))
							_Cubes[x][y][z].setDraw(false);
					}
		}


};