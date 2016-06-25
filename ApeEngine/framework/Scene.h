#ifndef _Scene_H
#define _Scene_H

class Input;
class D3DManager;
class Camera;
class Model;
class Texture;
class Light;
class Position;
class Timer;
class DeferredBuffers;
class OrthoWindow;
class DeferredShader;
class LightShader;

class Scene
{
public:
	Scene(D3DManager* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear);
	~Scene();

	bool Frame(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Input> pInput, std::shared_ptr<Texture> pTexture, float fFrameTime, int iFPS);

private:
	void HandleMovementInput(std::shared_ptr<Input> pInput, float fFrameTime);
	bool Render(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture);
	bool RenderDeferred(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture);
	bool RenderWindow(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture);

private:
	std::shared_ptr<Camera>			 m_pCamera;
	std::shared_ptr<DeferredShader>  m_pDeferredShader;
	std::shared_ptr<DeferredBuffers> m_pDeferredBuffers;
	std::shared_ptr<Model>			 m_pModel;
	std::shared_ptr<OrthoWindow>	 m_pFullScreenWindow;
	std::shared_ptr<LightShader>	 m_pLightShader;
	std::shared_ptr<Position>		 m_pPosition;
	std::shared_ptr<Light>		     m_pLight;

	DirectX::XMFLOAT3 m_CameraPos;

	bool m_bDisplayUI, m_bWireFrame, m_bCellLines, m_bHeightLocked;
	int m_iScreenWidth, m_iScreenHeight;
};

#endif
