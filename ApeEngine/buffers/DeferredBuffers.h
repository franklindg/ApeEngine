#ifndef _DEFERRED_BUFFERS_H
#define _DEFERRED_BUFFERS_H

const int BUFFER_COUNT = 4;

class DeferredBuffers
{
public:
	DeferredBuffers(ID3D11Device* pDevice, int iWidth, int iHeight, float fDepth, float fNear);
	~DeferredBuffers();

	bool Initialize(int iWidth, int iHeight, float fDepth, float fNear);

	void SetRenderTargets();
	void ClearRenderTargets(float fRed, float fGreen, float fBlue, float fAlpha);

	ID3D11ShaderResourceView* GetShaderResourceView(int);

private:
	int m_iWidth, m_iHeight;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	ID3D11Texture2D* m_paTexture[BUFFER_COUNT];
	ID3D11RenderTargetView* m_paRenderTarget[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_paShaderResource[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_Viewport;
};

#endif