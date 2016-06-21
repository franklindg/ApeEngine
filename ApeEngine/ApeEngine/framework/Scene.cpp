/////////////////////////////////////////////
// Filename: Scene.cpp
/////////////////////////////////////////////
#include "Scene.h"

Scene::Scene()
	: m_pUserInterface(0)
	, m_pCamera(nullptr)
	, m_pLight(0)
	, m_pPosition(0)
	, m_pModel(nullptr)
	, m_pFullScreenWindow(nullptr)
	, m_pDeferredBuffers(nullptr)
	, m_pDeferredShader(nullptr)
	, m_pLightShader(nullptr)
	, m_fRotation(0.0f)
{}

Scene::Scene(const Scene& other)
{

}

Scene::~Scene()
{

}

bool Scene::Initialize(D3DManager* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	bool result;

	/****** Initialize User Interface ******/
	m_pUserInterface = new UserInterface;
	if (!m_pUserInterface)
	{
		return false;
	}
	
	if (!m_pUserInterface->Initialize(Direct3D, screenHeight, screenWidth))
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	/****** Initialize Camera ******/
	m_pCamera = std::make_shared<Camera>();
	if (!m_pCamera)
	{
		return false;
	}

	m_pCamera->SetPosition(0.0f, 0.0f, -1.0f);
	m_pCamera->RenderBaseViewMatrix();

	/****** Initialize Light ******/
	m_pLight = new Light;
	if (!m_pLight)
	{
		return false;
	}

	m_pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight->SetDirection(0.0f, 0.0f, 1.0f);
	
	/****** Initialize Position ******/
	m_pPosition = new Position;
	if (!m_pPosition)
	{
		return false;
	}

	m_pPosition->SetPosition(0.0f, 0.0f, -10.0f);
	m_pPosition->SetRotation(0.0f, 0.0f, 0.0f);

	/****** Initialize Model ******/
	m_pModel = std::make_shared<Model>();
	if (!m_pModel)
	{
		return false;
	}

	result = m_pModel->Initialize(Direct3D->GetDevice(), "data/models/sphere.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	/****** Initialize OrthoWindow ******/
	m_pFullScreenWindow = std::make_shared<OrthoWindow>();
	if (!m_pFullScreenWindow)
	{
		return false;
	}

	result = m_pFullScreenWindow->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the orthographic window object.", L"Error", MB_OK);
		return false;
	}
	/****** Initialize DeferredBuffers ******/
	m_pDeferredBuffers = std::make_shared<DeferredBuffers>();
	if (!m_pDeferredBuffers)
	{
		return false;
	}

	result = m_pDeferredBuffers->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight, screenDepth, screenNear);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred buffer object.", L"Error", MB_OK);
		return false;
	}

	/****** Initialize DeferredShader ******/
	m_pDeferredShader = std::make_shared<DeferredShader>(Direct3D->GetDevice());
	if (!m_pDeferredShader)
	{
		return false;
	}

	result = m_pDeferredShader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred shader object.", L"Error", MB_OK);
		return false;
	}

	/****** Initialize LightShader ******/
	m_pLightShader = std::make_shared<LightShader>();
	if (!m_pLightShader)
	{
		return false;
	}

	result = m_pLightShader->Initialize(Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_bCellLines = true;
	m_bDisplayUI = true;
	m_bWireFrame = true;
	m_bHeightLocked = true;

	return true;
}

void Scene::Shutdown()
{
	if (m_pModel)
	{
		m_pModel->Shutdown();
		m_pModel.reset();
	}

	if (m_pPosition)
	{
		delete m_pPosition;
		m_pPosition = 0;
	}

	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = 0;
	}

	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();
		m_pLightShader.reset();
	}

	if (m_pDeferredShader)
	{
		m_pDeferredShader->Shutdown();
		m_pDeferredShader.reset();
	}

	if (m_pFullScreenWindow)
	{
		m_pFullScreenWindow->Shutdown();
		m_pFullScreenWindow.reset();
	}

	if (m_pDeferredBuffers)
	{
		m_pDeferredBuffers->Shutdown();
		m_pDeferredBuffers.reset();
	}

	if (m_pCamera)
	{
		m_pCamera.reset();
		m_pCamera = 0;
	}

	if (m_pUserInterface)
	{
		m_pUserInterface->Shutdown();
		delete m_pUserInterface;
		m_pUserInterface = 0;
	}

	return;
}

bool Scene::Frame(D3DManager* Direct3D, Input* input, ShaderManager* shaderManager, TextureManager* textureManager, float frameTime, int fps)
{
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Updates user movement input.
	HandleMovementInput(input, frameTime);
	m_pPosition->GetPosition(posX, posY, posZ);
	m_pPosition->GetRotation(rotX, rotY, rotZ);

	// Updates user interface variables.
	if (!m_pUserInterface->Frame(Direct3D->GetDeviceContext(), input, fps, posX, posY, posZ, rotX, rotY, rotZ))
		return false;
	
	// Updates the scene.
	if (!Render(Direct3D, shaderManager, textureManager))
		return false;

	// Updates rotation float.
	m_fRotation += (float)XM_PI * 0.003f;
	if (m_fRotation > 360.0f)
	{
		m_fRotation -= 360.0f;
	}

	return true;
}

void Scene::HandleMovementInput(Input* input, float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	m_pPosition->SetFrameTime(frameTime);

	keyDown = input->IsAPressed();			 // A - Key
	m_pPosition->MoveLeft(keyDown);			 // -- Move Left
											 // ****************
	keyDown = input->IsDPressed();			 // D - Key
	m_pPosition->MoveRight(keyDown);		 // -- Move Right
											 // ****************
	keyDown = input->IsWPressed();			 // W - Key
	m_pPosition->MoveForward(keyDown);		 // -- Move Forward
											 // ****************
	keyDown = input->IsSPressed();			 // S - Key
	m_pPosition->MoveBackward(keyDown);		 // -- Move Backward
											 // ****************
	keyDown = input->IsRightPressed();		 // Right Arrow - Key
	m_pPosition->TurnRight(keyDown);		 // -- Turn Right
											 // ****************
	keyDown = input->IsLeftPressed();		 // Left Arrow - Key
	m_pPosition->TurnLeft(keyDown);			 // -- Turn Left
											 // ****************
	keyDown = input->IsUpPressed();			 // Up Arrow - Key
	m_pPosition->MoveUpward(keyDown);		 // -- Move Up
											 // ****************
	keyDown = input->IsDownPressed();		 // Down Arrow - Key
	m_pPosition->MoveDownward(keyDown);		 // -- Move Down
											 // ****************
	keyDown = input->IsPgUpPressed();		 // PgUp - Key
	m_pPosition->LookUpward(keyDown);		 // -- Look Up
											 // ****************
	keyDown = input->IsPgDownPressed();		 // PgDown - Key
	m_pPosition->LookDownward(keyDown);		 // -- Look Down

	// Updates the change in position from user input.
	m_pPosition->GetPosition(posX, posY, posZ);
	m_pPosition->GetRotation(rotX, rotY, rotZ);

	// Sets the camera position equal to the user interface position.
	m_pCamera->SetPosition(posX, posY, posZ);
	m_pCamera->SetRotation(rotX, rotY, rotZ);

	if (input->IsF1Toggled())				  // ************************
	{										  // F1 - Key
		m_bDisplayUI = !m_bDisplayUI;		  // -- Toggle User Interface
	}										  // ************************

	if (input->IsF2Toggled())				  // ************************						  
	{										  // F2 - Key
		m_bWireFrame = !m_bWireFrame;		  // -- Toggle WireFrame
	}										  // ************************
											  
	if (input->IsF3Toggled())				  // ************************
	{										  // F3 - Key
		m_bCellLines = !m_bCellLines;		  // -- Toggle Cell Lines
	}										  // ************************
											  
	if (input->IsF4Toggled())				  // ************************
	{										  // F4 - Key
		m_bHeightLocked = !m_bHeightLocked;	  // -- Toggle HeightLock
	}										  // ************************

	return;
}

bool Scene::Render(D3DManager* Direct3D, ShaderManager* shaderManager, TextureManager* textureManager)
{	
	if(!RenderDeferred(Direct3D, shaderManager, textureManager))
		return false;

	if(!RenderWindow(Direct3D, shaderManager, textureManager))
		return false;
	
	return true;
}

bool Scene::RenderDeferred(D3DManager* Direct3D, ShaderManager* shaderManager, TextureManager* textureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT3 cameraPosition;

	

	// Gets matrices for model rendering.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	
	
	
	

	// Sets and clears the buffers to be rendered to.
	m_pDeferredBuffers->SetRenderTargets(Direct3D->GetDeviceContext());
	m_pDeferredBuffers->ClearRenderTargets(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_pCamera->Render();

	
	// Renders the models buffer.
	m_pModel->Render(Direct3D->GetDeviceContext());	

	worldMatrix = XMMatrixRotationY(m_fRotation);

	// Renders the deferred shader.
	m_pDeferredShader->Render(m_pModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, 
		textureManager->GetTexture(2), textureManager->GetTexture(3));

	// Resets the back buffer and view-port.
	Direct3D->SetBackBufferRenderTarget();
	Direct3D->ResetViewport();
	
	return true;
}

bool Scene::RenderWindow(D3DManager* Direct3D, ShaderManager* shaderManager, TextureManager* textureManager)
{
	XMMATRIX WorldMatrix, BaseViewMatrix, OrthoMatrix;

	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Gets the matrices for the render window.
	Direct3D->GetWorldMatrix(WorldMatrix);
	m_pCamera->GetBaseViewMatrix(BaseViewMatrix);
	Direct3D->GetOrthoMatrix(OrthoMatrix);

	Direct3D->TurnZBufferOff();

	m_pFullScreenWindow->Render(Direct3D->GetDeviceContext());

	m_pLightShader->Render(Direct3D->GetDeviceContext(),
		m_pFullScreenWindow->GetIndexCount(),
		WorldMatrix, BaseViewMatrix, OrthoMatrix,
		m_pDeferredBuffers->GetShaderResourceView(0),
		m_pDeferredBuffers->GetShaderResourceView(1),
		m_pDeferredBuffers->GetShaderResourceView(2),
		m_pDeferredBuffers->GetShaderResourceView(3),
		m_pCamera->GetPosition(), m_pLight->GetDirection());

	if (m_bDisplayUI)
	{
		
		if (!m_pUserInterface->Render(Direct3D, shaderManager, WorldMatrix, BaseViewMatrix, OrthoMatrix))
		{
			return false;
		}
	}

	Direct3D->TurnZBufferOn();
	Direct3D->EndScene();

	return true;
}

//void Scene::TestIntersection(D3DManager* Direct3D, Camera* camera, int mouseX, int mouseY)
//{
//	float pointX, pointY;
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, inverseViewMatrix, inverseWorldMatrix;
//	XMFLOAT3 direction, origin, rayOrigin, rayDirection;
//	bool intersect;
//
//	pointX = ((2.0f * (float)mouseX) / (float)m_iScreenWidth) - 1.0f;
//	pointY = (((2.0f * (float)mouseY) / (float)m_iScreenHeight) - 1.0f) * -1.0f;
//
//	Direct3D->GetProjectionMatrix(projectionMatrix);
//	pointX = pointX / projectionMatrix._11;
//	pointY = pointY / projectionMatrix._22;
//
//	camera->GetViewMatrix(viewMatrix);
//	XMStoreFloat4x4(&inverseViewMatrix, (XMMatrixInverse(NULL, XMLoadFloat4x4(&viewMatrix))));
//
//	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
//	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
//	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;
//
//	origin = camera->GetPosition();
//
//	Direct3D->GetWorldMatrix(worldMatrix);
//	XMStoreFloat4x4(&worldMatrix, (XMLoadFloat4x4(&worldMatrix) = XMMatrixTranslation(-5.0f, 1.0f, 5.0f)));
//	XMStoreFloat4x4(&inverseWorldMatrix, (XMMatrixInverse(NULL, XMLoadFloat4x4(&worldMatrix))));
//
//	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMLoadFloat3(&origin), XMLoadFloat4x4(&inverseWorldMatrix)));
//	XMStoreFloat3(&rayDirection, XMVector3TransformNormal(XMLoadFloat3(&direction), XMLoadFloat4x4(&inverseWorldMatrix)));
//	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMLoadFloat3(&rayDirection)));
//
//	intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);
//}

//bool Scene::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
//{
//	float a, b, c, discriminant;
//
//	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
//	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
//	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);
//
//	discriminant = (b * b) - (4 * a * c);
//
//	if (discriminant < 0.0f)
//	{
//		return false;
//	}
//
//	return true;
//}

