#ifndef _DEFERRED_BUFFERS_H
#define _DEFERRED_BUFFERS_H

const int BUFFER_COUNT = 4;

class DeferredBuffers
{
public:
	DeferredBuffers(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear);
	~DeferredBuffers();

	bool Initialize(int textureWidth, int textureHeight, float screenDepth, float screenNear);

	void SetRenderTargets();
	void ClearRenderTargets(float red, float green, float blue, float alpha);

	ID3D11ShaderResourceView* GetShaderResourceView(int);

private:
	int m_textureWidth, m_textureHeight;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	ID3D11Texture2D* m_renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViewArray[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceViewArray[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
};

#endif