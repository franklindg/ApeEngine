/////////////////////////////////////////////
// Filename: ModelList.h
/////////////////////////////////////////////
#ifndef _MODEL_LIST_H
#define _MODEL_LIST_H

//////////
// INCLUDES //
//////////
#include <time.h>
#include <stdlib.h>
#include <DirectXMath.h>
using namespace DirectX;


///////////////////
// Class name: ModelList //
///////////////////
class ModelList
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};

public:
	ModelList();
	ModelList(const ModelList&);
	~ModelList();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, XMFLOAT4&);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;

};

#endif