/////////////////////////////////////////////
// Filename: Scene.h
/////////////////////////////////////////////
#ifndef _Scene_H
#define _Scene_H

/////////////
// CLASS INCLUDES //
/////////////
#include "D3DManager.h"
#include "Input.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "../render3d/Model.h"
#include "../utilities/Timer.h"
#include "../utilitiesfrw/Light.h"
#include "../utilitiesfrw/Position.h"
#include "../utilitiesfrw/DeferredBuffers.h"
#include "../shader/DeferredShader.h"
#include "../utilitiesfrw/OrthoWindow.h"
#include "../shader/LightShader.h"

///////////////////
// Class name: Scene //
///////////////////
class Scene
{
public:
	Scene(D3DManager* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear);
	~Scene();

	bool Frame(D3DManager*, Input*, ShaderManager*, Texture* texture, float, int);

private:
	void HandleMovementInput(Input*, float);
	bool Render(D3DManager*, ShaderManager*, Texture* texture);
	bool RenderDeferred(D3DManager*, ShaderManager*, Texture* texture);
	bool RenderWindow(D3DManager*, ShaderManager*, Texture* texture);
	void TestIntersection(D3DManager*, Camera*, int, int);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);

private:
	std::shared_ptr<Camera>			 m_pCamera;
	std::shared_ptr<DeferredShader>  m_pDeferredShader;

	std::shared_ptr<DeferredBuffers> m_pDeferredBuffers;
	std::shared_ptr<Model>			 m_pModel;
	std::shared_ptr<OrthoWindow>	 m_pFullScreenWindow;
	std::shared_ptr<LightShader>	 m_pLightShader;
	std::shared_ptr<Position>		 m_pPosition;
	std::shared_ptr<Light>		     m_pLight;

	XMFLOAT3 m_CameraPos;

	bool m_bDisplayUI, m_bWireFrame, m_bCellLines, m_bHeightLocked;
	int m_iScreenWidth, m_iScreenHeight;
	float m_fRotation;
};

#endif
