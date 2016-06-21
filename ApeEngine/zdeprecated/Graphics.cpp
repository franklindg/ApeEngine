///////////////////////////////////////////////
//// Filename: Graphics.cpp
///////////////////////////////////////////////
//#include "Graphics.h"
//
//Graphics::Graphics()
//{
//	m_Direct3D = 0;
//	m_Camera = 0;
//	m_Model = 0;
//	m_TextureShader = 0;
//	m_LightShader = 0;
//	m_Light = 0;
//	m_Bitmap = 0;
//}
//
//Graphics::Graphics(const Graphics& other)
//{
//
//}
//
//Graphics::~Graphics()
//{
//
//}
//
//bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
//{
//	bool result;
//
//	// Create the Direct3D object.
//	m_Direct3D = new D3DManager;
//	if (!m_Direct3D)
//	{
//		return false;
//	}
//
//	// Initialize the Direct3D object.
//	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the camera object.
//	m_Camera = new Camera;
//	if (!m_Camera)
//	{
//		return false;
//	}
//
//	// Set the initial position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
//
//	// Create the model object.
//	m_Model = new Model;
//	if (!m_Model)
//	{
//		return false;
//	}
//
//	// Initialize the model object.
//	result = m_Model->Initialize(m_Direct3D->GetDevice(), "data/models/monkey.txt");
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the light shader object.
//	m_LightShader = new LightShader;
//	if (!m_LightShader)
//	{
//		return false;
//	}
//
//	// Initialize the light shader object.
//	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the light object.
//	m_Light = new Light;
//	if (!m_Light)
//	{
//		return false;
//	}
//
//	// Initialize the light object.
//	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
//	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Light->SetDirection(0.0f, 1.0f, 1.0f);
//	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Light->SetSpecularPower(32.0f);
//
//	// Create the texture shader object.
//	m_TextureShader = new TextureShader;
//	if (!m_TextureShader)
//	{
//		return false;
//	}
//
//	// Initialize the texture shader object.
//	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the bitmap object.
//	m_Bitmap = new Bitmap;
//	if (!m_Bitmap)
//	{
//		return false;
//	}
//	
//	// Initialize the bitmap object.
//	//result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, "data/textures/stone01.tga", 512, 512);
//	//if (!result)
//	//{
//	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
//	//	return false;
//	//}
//	//
//	//return true;
//}
//
//void Graphics::Shutdown()
//{
//	// Release the bitmap object.
//	if (m_Bitmap)
//	{
//		m_Bitmap->Shutdown();
//		delete m_Bitmap;
//		m_Bitmap = 0;
//	}
//
//	// Release the light object.
//	if (m_Light)
//	{
//		delete m_Light;
//		m_Light = 0;
//	}
//
//	// Release the light shader object.
//	if (m_LightShader)
//	{
//		m_LightShader->Shutdown();
//		delete m_LightShader;
//		m_LightShader = 0;
//	}
//
//	// Release the model object.
//	if (m_Model)
//	{
//		m_Model->Shutdown();
//		delete m_Model;
//		m_Model = 0;
//	}
//
//	// Release the camera object.
//	if (m_Camera)
//	{
//		delete m_Camera;
//		m_Camera = 0;
//	}
//
//	// Release the Direct3D object.
//	if (m_Direct3D)
//	{
//		m_Direct3D->Shutdown();
//		delete m_Direct3D;
//		m_Direct3D = 0;
//	}
//
//	return;
//}
//
//bool Graphics::Frame(int mouseX, int mouseY)
//{
//	bool result;
//	//static float rotation = 0.0f;
//
//	//// Update the rotation variable each frame.
//	//rotation += (float)XM_PI * 0.005f;
//	//if (rotation > 360.0f)
//	//{
//	//	rotation -= 360.0f;
//	//}
//
//	//// Render the graphics scene.
//	//result = Render(rotation);
//	//if (!result)
//	//{
//	//	return false;
//	//}
//	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
//
//	return true;
//}
//
//bool Graphics::Render(float rotation)
//{
//	XMFLOAT4X4 worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
//	bool result;
//
//	// Clear the buffers to begin the scene.
//	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Generate the view matrix based on the camera's position.
//	m_Camera->Render();
//
//	// Get the world, view, and projection matrices from the camera and d3d objects.
//	m_Camera->GetViewMatrix(viewMatrix);
//	m_Direct3D->GetWorldMatrix(worldMatrix);
//	m_Direct3D->GetProjectionMatrix(projectionMatrix);
//	m_Direct3D->GetOrthoMatrix(orthoMatrix);
//
//	// Turn on the Z buffer to begin all 2D rendering.
//	m_Direct3D->TurnZBufferOff();
//
//	// Turn on the alpha blending before rendering text.
//	m_Direct3D->TurnAlphaBlendingOn();
//
//	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100);
//	if (!result)
//	{
//		return false;
//	}
//	
//	// Render the bitmap with the texture shader.
//	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
//	if (!result)
//	{
//		return false;
//	}
//
//	// Turn off alpha blending after rendering the text.
//	m_Direct3D->TurnAlphaBlendingOff();
//	
//	// Turn the Z buffer back on now that all 2D rendering has completed.
//	m_Direct3D->TurnZBufferOn();
//
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_Model->Render(m_Direct3D->GetDeviceContext());
//	
//	// Render the model using the light shader.
//	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
//		, m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
//		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
//	if (!result)
//	{
//		return false;
//	}
//	
//	// Present the rendered scene to the screen.
//	m_Direct3D->EndScene();
//
//	return true;
//}