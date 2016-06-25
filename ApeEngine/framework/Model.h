#ifndef _MODEL_H
#define _MODEL_H

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
		UINT NumIndices;
	};

	struct VertexType
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 bitangent;
	};

public:
	Model(ID3D11Device* device, const char* filename);
	~Model();

	void Render();

	UINT GetIndexCount();

private:
	bool InitializeBuffers(const char* filename);
	void RenderBuffers();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	ModelData mModel;
};

#endif