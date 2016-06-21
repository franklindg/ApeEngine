/////////////////////////////////////////////
// Filename: DeferredBuffers.h
/////////////////////////////////////////////
#ifndef _DEFERRED_BUFFERS_H
#define _DEFERRED_BUFFERS_H

//////////
// DEFINES //
//////////
const int BUFFER_COUNT = 4;

//////////
// INCLUDES //
//////////
#include "../framework/ApePCH.h"
using namespace DirectX;

///////////////////
// Class name: DeferredBuffer //
///////////////////
class DeferredBuffers
{
public:
	DeferredBuffers();
	DeferredBuffers(const DeferredBuffers&);
	~DeferredBuffers();

	bool Initialize(ID3D11Device*, int, int, float, float);
	void Shutdown();

	void SetRenderTargets(ID3D11DeviceContext*);
	void ClearRenderTargets(ID3D11DeviceContext*, float, float, float, float);

	ID3D11ShaderResourceView* GetShaderResourceView(int);

private:
	int m_textureWidth, m_textureHeight;
	ID3D11Texture2D* m_renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViewArray[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceViewArray[BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
};

#endif