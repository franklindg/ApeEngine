/////////////////////////////////////////////
// Filename: Model.h
/////////////////////////////////////////////
#ifndef _MODEL_H
#define _MODEL_H


//////////
// INCLUDES //
//////////
#include "../framework/ApePCH.h"
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

////////
// CLASS INCLUDES //
////////
#include "../utilitiesfrw/Texture.h"
#include "../framework/TextureManager.h"
#include "../utilitiesfrw/Materials.h"

///////////////////
// Class name: Model //
///////////////////
class Model
{
private:
	struct ModelData
	{
		ModelData()
		{
			VertexBuffer = nullptr;
			IndexBuffer = nullptr;
		}

		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;

		UINT NumVertices;
		UINT NumFaces;
	};

	struct VertexType
	{
		XMFLOAT4 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 bitangent;
	};

public:
	Model(ID3D11Device* device, const char* filename);
	Model(const Model&);
	~Model();

	void Render(ID3D11DeviceContext*);

	UINT GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, const char*);
	void RenderBuffers(ID3D11DeviceContext*);
	void ImportMaterials(const aiMaterial& material);

private:
	ModelData mModel;
};

#endif