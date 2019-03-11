#ifndef VBO_H
#define VBO_H

#include "external/gl/glew.h"
#include "external/gl/freeglut.h" 
#include "engine/utils/utils.h"
#include "engine/utils/types_3d.h"
#include "engine/render/FboCustom.h"
#include "engine/render/GBuffer.h"

class YVbo
{
public:

	//D�finit un �l�ment du VBO
	struct Element {
		Element(int nbFloats) 
		{ 
			NbFloats = nbFloats; 
		}
		Element()
		{
			NbFloats = 0;
		}
		int NbFloats = 0; ///< Choisi par l'utilsateur
		int OffsetFloats = 0; ///< Calcul� automatiquement
	};

	template <typename T>
	struct MemorySegment
	{
		T * begin;
		T * end;
	};

	typedef enum {
		PACK_BY_VERTICE,
		PACK_BY_ELEMENT_TYPE,
	}DATA_STORAGE_METHOD;

protected:
	Element * Elements = NULL; ///<La description des diff�rents elements du VBO : une coord de sommet, une normale, une  uv, un float...
	int NbElements; ///< Le nomre d'elements diff�rents qu'on a dans le VBO
	float * ElementsValues = NULL; ///< Des tableaux qui contiennent les valeurs des �l�ments
	int NbVertices; ///< Le nombre de sommets qu'on a dans le VBO
	int TotalSizeFloats; ///< Taille totale du VBO en floats
	int TotalNbFloatForOneVertice; ///< Taille totale d'un vertice (avec tous ces elements) en floats
	GLuint VBO; ///< L'identifiant du VBO pour opengl
	GLuint VAO; ///< L'identifiant du VAO (description des datas) pour opengl
	DATA_STORAGE_METHOD StorageMethod = PACK_BY_ELEMENT_TYPE; ///< Comment on range les datas dans le VBO
	bool isArrayIndex;
	int heightTex;
	int widthTex;
	unsigned int textureIndex;
	unsigned int textureCubeIndex;
	YVbo * index;

public:		
		
	//On cr�e un VBO en lui passant les �l�ments qu'il contient
	YVbo(int nbElements, int nbVertices, DATA_STORAGE_METHOD storageMethod, bool index = false)
	{
		this->Elements = new Element[nbElements];
		NbElements = nbElements;
		NbVertices = nbVertices;
		StorageMethod = storageMethod;
		isArrayIndex = index;
		textureIndex = 0;
		widthTex = 0;
		heightTex = 0;
		VBO = 0;
	}

	~YVbo()
	{
		SAFEDELETE_TAB(this->Elements);
		SAFEDELETE_TAB(ElementsValues);
		if (VAO != 0)
			glDeleteVertexArrays(1, &VAO);
		if (VBO != 0)
			glDeleteBuffers(1, &VBO);
	}

	int getVboSizeBytes() {
		return TotalSizeFloats * sizeof(float);
	}

	const unsigned int GetTextureIndex()
	{
		return textureIndex;
	}

	const unsigned int GetTextureCubeIndex()
	{
		return textureCubeIndex;
	}

	int getNbVertices() {
		return NbVertices;
	}

	float * getElement(unsigned int index)
	{
		return &ElementsValues[index];
	}

	MemorySegment<float> getElement(unsigned int index, unsigned int nbList)
	{
		MemorySegment<float> output;
		output.begin = &ElementsValues[Elements[nbList].OffsetFloats + (TotalNbFloatForOneVertice * index)];
		output.end = output.begin + (Elements[nbList].NbFloats * sizeof(float));
		return output;
	}

	bool isArrayIndexType()
	{
		return isArrayIndex;
	}

	unsigned int * getIndexVBO()
	{
		return &VBO;
	}

	//On set les types d'elements que contient le VBO. On a set le nombre d'elements a la cr�ation
	void setElementDescription(int iElement, const Element & element) {
		this->Elements[iElement] = element;

		TotalSizeFloats = 0;
		TotalNbFloatForOneVertice = 0;
		for (int i = 0; i < NbElements; i++) {
			if(StorageMethod == PACK_BY_ELEMENT_TYPE)
				Elements[i].OffsetFloats = TotalSizeFloats;
			else
				Elements[i].OffsetFloats = TotalNbFloatForOneVertice;
				
			TotalNbFloatForOneVertice += Elements[i].NbFloats;
			TotalSizeFloats += NbVertices * Elements[i].NbFloats;
		}
				
	}

	void SetTexture(unsigned int index)
	{
		textureIndex = index;
		glBindTexture(GL_TEXTURE_2D, index);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SetTextureCube(unsigned int cubeIndex)
	{
		textureCubeIndex = cubeIndex;
	}

	//Cr�ation des buffers en RAM pour stoquer toutes les valeurs
	void createVboCpu() {
		SAFEDELETE_TAB(ElementsValues);
		ElementsValues = new float[TotalSizeFloats];
	}

	virtual void deleteVboCpu() {
		SAFEDELETE_TAB(ElementsValues);
		ElementsValues = NULL;
	}

	//Permet de set un element qui fait un float de long
	void setElementValue(int iElement, int iValue, float f1)
	{
		if (StorageMethod == PACK_BY_ELEMENT_TYPE)
			ElementsValues[Elements[iElement].OffsetFloats + iValue] = f1;
		else
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue] = f1;
	}

	//Permet de set un element qui fait 2 float de long
	void setElementValue(int iElement, int iValue, float f1, float f2)
	{
		if (StorageMethod == PACK_BY_ELEMENT_TYPE) {
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 2 + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 2 + 1] = f2;
		}
		else {
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 1] = f2;
		}
			
	}

	//Permet de set un element qui fait 3 float de long
	void setElementValue(int iElement, int iValue, float f1, float f2,float f3)
	{
		if (StorageMethod == PACK_BY_ELEMENT_TYPE) {
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 3 + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 3 + 1] = f2;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 3 + 2] = f3;
		}
		else {
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 1] = f2;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 2] = f3;
		}
	}

	//Permet de set un element qui fait 4 float de long
	void setElementValue(int iElement, int iValue, float f1, float f2, float f3, float f4)
	{
		if (StorageMethod == PACK_BY_ELEMENT_TYPE) {
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 4 + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 4 + 1] = f2;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 4 + 2] = f3;
			ElementsValues[Elements[iElement].OffsetFloats + iValue * 4 + 3] = f4;
		}
		else {
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 0] = f1;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 1] = f2;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 2] = f3;
			ElementsValues[Elements[iElement].OffsetFloats + TotalNbFloatForOneVertice * iValue + 3] = f4;
		}
	}

	void SetFace(YVec3f origin, YVec3f directionFirst, YVec3f directionSecond, float size, int &startingIndex, bool drawQuad = false, int nbList = 0)
	{
		YVec3f destination1 = (origin + (directionFirst.normalize() * size));
		YVec3f destination2 = (origin + (directionSecond.normalize() * size));

		setElementValue(nbList, startingIndex, origin.X, origin.Y, origin.Z);
		setElementValue(nbList, startingIndex + 1, destination1.X, destination1.Y, destination1.Z);
		setElementValue(nbList, startingIndex + 2, destination2.X, destination2.Y, destination2.Z);

		if (drawQuad)
		{
			YVec3f destination3 = origin + (directionFirst.normalize() * size) + (directionSecond.normalize() * size);
			setElementValue(nbList, startingIndex + 3, destination3.X, destination3.Y, destination3.Z);
			setElementValue(nbList, startingIndex + 4, destination2.X, destination2.Y, destination2.Z);
			setElementValue(nbList, startingIndex + 5, destination1.X, destination1.Y, destination1.Z);
		}
		startingIndex += 6;
	}

	void SetNormale(YVec3f directionFirst, YVec3f directionSecond, int &startingIndex, int nbList = 0, bool isQuad = true)
	{
		YVec3f normale(directionFirst.cross(directionSecond));
		for (int i = 0; i < (isQuad ? 6 : 3); i++)
		{
			setElementValue(nbList, startingIndex + i, normale.X, normale.Y, normale.Z);
		}	
		startingIndex += isQuad ?6 : 3;
	}

	void SetTextureElt(int &startingIndex, std::vector<std::pair<int,int>> &UVs,float &valX,float &valY,float width = 1, float height = 1, int index = 0, int nbList = 0)
	{
		float XUnit(1 / width);
		float YUnit(1 / height);

		std::pair<int, int> valueUV(UVs[index]);
		setElementValue(nbList, startingIndex, (valueUV.first * valX) + (XUnit * 3), ((valueUV.second + 1) * valY) - (YUnit * 3));
		setElementValue(nbList, startingIndex + 1, ((valueUV.first + 1) * valX) - (XUnit * 3), ((valueUV.second + 1) * valY) - (YUnit * 3));
		setElementValue(nbList, startingIndex + 2, (valueUV.first * valX) + (XUnit * 3), (valueUV.second * valY) + (YUnit * 3));
		setElementValue(nbList, startingIndex + 3, ((valueUV.first + 1) * valX) - (XUnit * 3), (valueUV.second * valY) + (YUnit * 3));
		setElementValue(nbList, startingIndex + 4, (valueUV.first * valX) + (XUnit * 3), (valueUV.second * valY) + (YUnit * 3));
		setElementValue(nbList, startingIndex + 5,((valueUV.first + 1) * valX) - (XUnit * 3), ((valueUV.second + 1)* valY) - (YUnit * 3));
		startingIndex += 6;
	}

	//Creation et copie du VBO dans la m�moire du GPU
	virtual void createVboGpu();
	virtual void render(GBuffer * inBuffer = NULL);
};

#endif
