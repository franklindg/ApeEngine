#ifndef _ORTHO_WINDOW_H_
#define _ORTHO_WINDOW_H_

class OrthoWindow
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	OrthoWindow(ID3D11Device* device, int windowWidth, int windowHeight);
	~OrthoWindow();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(int windowWidth, int windowHeight);
	void ShutdownBuffers();
	void RenderBuffers();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	int m_iVertexCount, m_iIndexCount;
};

#endif
