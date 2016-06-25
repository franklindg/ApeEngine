#ifndef _D3D_MANAGER_H
#define _D3D_MANAGER_H


using namespace DirectX;

///////////////////
// Class name: D3DManager //
///////////////////
class D3DManager
{
public:
	D3DManager(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
			   float screenDepth, float screenNear);
	~D3DManager();

	bool Initialize(int, int, bool, HWND, bool, float, float);

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnCulling();
	void TurnOffCulling();

	void TurnAlphaBlendingOn();
	void EnableAlphaToCoverageBlending();
	void TurnAlphaBlendingOff();

	void EnableWireFrame();
	void DisableWireFrame();

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11RasterizerState* m_rasterStateWireframe;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	ID3D11BlendState* m_alphaEnableBlendingState2;
	D3D11_VIEWPORT m_viewport;
};

#endif
