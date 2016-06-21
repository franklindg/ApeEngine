/////////////////////////////////////////////
// Filename: RenderTexture.h
/////////////////////////////////////////////
#ifndef _RENDER_TEXTURE_H
#define _RENDER_TEXTURE_H


//////////
// INCLUDES //
//////////
#include <d3d11_2.h>


///////////////////
// Class name: RenderTexture //
///////////////////
class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};

#endif