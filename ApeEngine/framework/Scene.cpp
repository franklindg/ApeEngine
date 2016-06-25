#include <ApePCH.h>
#include "Scene.h"

Scene::Scene(D3DManager* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear)
	: m_pCamera(nullptr)
	, m_pLight(nullptr)
	, m_pPosition(nullptr)
	, m_pModel(nullptr)
	, m_pFullScreenWindow(nullptr)
	, m_pDeferredBuffers(nullptr)
	, m_pDeferredShader(nullptr)
	, m_pLightShader(nullptr)
{
	// Initialize the scene objects.
	m_pCamera = std::make_shared<Camera>();
	m_pLight = std::make_shared<Light>();
	m_pPosition = std::make_shared<Position>();
	m_pModel = std::make_shared<Model>(Direct3D->GetDevice(), "data/models/sphere.obj");
	m_pFullScreenWindow = std::make_shared<OrthoWindow>(Direct3D->GetDevice(), screenWidth, screenHeight);
	m_pDeferredBuffers = std::make_shared<DeferredBuffers>(Direct3D->GetDevice(), screenWidth, screenHeight, screenDepth, screenNear);
	m_pDeferredShader = std::make_shared<DeferredShader>(Direct3D->GetDevice());
	m_pLightShader = std::make_shared<LightShader>(Direct3D->GetDevice(), hwnd);

	// Sets the camera position and renders the BaseViewMatrix for viewing.
	m_pCamera->SetPosition(0.0f, 0.0f, -1.0f);
	m_pCamera->RenderBaseViewMatrix();

	// Sets the lighting buffer.
	m_pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight->SetDirection(0.0f, 0.0f, 1.0f);

	// Sets the user position.
	m_pPosition->SetPosition(0.0f, 0.0f, -10.0f);
	m_pPosition->SetRotation(0.0f, 0.0f, 0.0f);
}

Scene::~Scene()
{
	m_pModel.reset();
	m_pPosition.reset();
	m_pLight.reset();
	m_pLightShader.reset();
	m_pDeferredShader.reset();
	m_pFullScreenWindow.reset();
	m_pDeferredBuffers.reset();
	m_pCamera.reset();
}

bool Scene::Frame(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Input> pInput, std::shared_ptr<Texture> pTexture, float fFrameTime, int iFPS)
{
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Updates user movement input.
	HandleMovementInput(pInput, fFrameTime);
	m_pPosition->GetPosition(posX, posY, posZ);
	m_pPosition->GetRotation(rotX, rotY, rotZ);
	
	// Updates the scene.
	if (!Render(pD3DManager, pTexture))
	{
		return false;
	}

	return true;
}

void Scene::HandleMovementInput(std::shared_ptr<Input> pInput, float fFrameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	m_pPosition->SetFrameTime(fFrameTime);

	// User movement input
	keyDown = pInput->IsAPressed();		// A - Key
	m_pPosition->MoveLeft(keyDown);		// Move Left
										
	keyDown = pInput->IsDPressed();		// D - Key
	m_pPosition->MoveRight(keyDown);	// Move Right
										
	keyDown = pInput->IsWPressed();		// W - Key
	m_pPosition->MoveForward(keyDown);	//  Move Forward
										
	keyDown = pInput->IsSPressed();		// S - Key
	m_pPosition->MoveBackward(keyDown);	// Move Backward
										
	keyDown = pInput->IsRightPressed();	// Right Arrow - Key
	m_pPosition->TurnRight(keyDown);	// Turn Right
										
	keyDown = pInput->IsLeftPressed();	// Left Arrow - Key
	m_pPosition->TurnLeft(keyDown);		// Turn Left
										
	keyDown = pInput->IsUpPressed();		// Up Arrow - Key
	m_pPosition->MoveUpward(keyDown);	// Move Up
										
	keyDown = pInput->IsDownPressed();	// Down Arrow - Key
	m_pPosition->MoveDownward(keyDown);	// Move Down
										
	keyDown = pInput->IsPgUpPressed();	// PgUp - Key
	m_pPosition->LookUpward(keyDown);	// Look Up
										
	keyDown = pInput->IsPgDownPressed();	// PgDown - Key
	m_pPosition->LookDownward(keyDown);	// Look Down

	// Updates the change in position from user input.
	m_pPosition->GetPosition(posX, posY, posZ);
	m_pPosition->GetRotation(rotX, rotY, rotZ);

	// Sets the camera position equal to the user interface position.
	m_pCamera->SetPosition(posX, posY, posZ);
	m_pCamera->SetRotation(rotX, rotY, rotZ);

	// Interface Toggles
	if (pInput->IsF1Toggled())				 
	{								  // F1 - Key
		m_bDisplayUI = !m_bDisplayUI; // Toggle User Interface
	}								

	if (pInput->IsF2Toggled())		
	{								  // F2 - Key
		m_bWireFrame = !m_bWireFrame; // Toggle WireFrame
	}								
									
	if (pInput->IsF3Toggled())		
	{								  // F3 - Key
		m_bCellLines = !m_bCellLines; // Toggle Cell Lines
	}
											
	if (pInput->IsF4Toggled())				
	{										// F4 - Key
		m_bHeightLocked = !m_bHeightLocked;	// Toggle HeightLock
	}										 
}

bool Scene::Render(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture)
{	
	if(!RenderDeferred(pD3DManager, pTexture))
	{
		return false;
	}
	if(!RenderWindow(pD3DManager, pTexture))
	{
		return false;
	}
	
	return true;
}

bool Scene::RenderDeferred(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT3 cameraPosition;

	// Gets matrices for model rendering.
	pD3DManager->GetWorldMatrix(worldMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);
	pD3DManager->GetProjectionMatrix(projectionMatrix);

	// Sets and clears the buffers to be rendered to.
	m_pDeferredBuffers->SetRenderTargets();
	m_pDeferredBuffers->ClearRenderTargets(0.0f, 0.0f, 0.0f, 1.0f);

	m_pCamera->Render();

	// Renders the models buffer.
	m_pModel->Render();	

	// Renders the deferred shader.
	if (!m_pDeferredShader->Render(m_pModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		pTexture->GetTexture(0),
		pTexture->GetTexture(1)))
	{
		return false;
	}

	// Resets the back buffer and view-port.
	pD3DManager->SetBackBufferRenderTarget();
	pD3DManager->ResetViewport();
	
	return true;
}

bool Scene::RenderWindow(std::shared_ptr<D3DManager> pD3DManager, std::shared_ptr<Texture> pTexture)
{
	XMMATRIX WorldMatrix, BaseViewMatrix, OrthoMatrix;

	pD3DManager->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Gets the matrices for the render window.
	pD3DManager->GetWorldMatrix(WorldMatrix);
	m_pCamera->GetBaseViewMatrix(BaseViewMatrix);
	pD3DManager->GetOrthoMatrix(OrthoMatrix);

	// Turns the Z-Buffer off to allow 2D rendering.
	pD3DManager->TurnZBufferOff();

	// Render the render target.
	m_pFullScreenWindow->Render(pD3DManager->GetDeviceContext());

	if (!m_pLightShader->Render(m_pFullScreenWindow->GetIndexCount(),
		WorldMatrix, BaseViewMatrix, OrthoMatrix,
		m_pDeferredBuffers->GetShaderResourceView(0),
		m_pDeferredBuffers->GetShaderResourceView(1),
		m_pDeferredBuffers->GetShaderResourceView(2),
		m_pDeferredBuffers->GetShaderResourceView(3),
		m_pCamera->GetPosition(),
		m_pLight->GetDirection()))
	{
		return false;
	}

	// Turns the Z-Buffer on to allow 3D rendering.
	pD3DManager->TurnZBufferOn();

	// Presents the processed scene to the backbuffer.
	pD3DManager->EndScene();

	return true;
}
