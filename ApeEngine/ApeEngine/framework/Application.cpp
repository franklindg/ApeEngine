/////////////////////////////////////////////
// Filename: Application.cpp
/////////////////////////////////////////////
#include "Application.h"

Application::Application()
	: m_Input(nullptr)
	, m_Direct3D(nullptr)
	, m_Timer(nullptr)
	, m_Fps(nullptr)
	, m_ShaderManager(nullptr)
	, m_TextureManager(nullptr)
	, m_Scene(nullptr)
{}

Application::Application(const Application& other)
{

}

Application::~Application()
{

}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	
	/* Input Class */
	m_Input = std::make_shared<Input>(hinstance, hwnd, screenWidth, screenHeight);
	if (!m_Input)
	{
		return false;
	}

	/* D3DManager Class */
	m_Direct3D = std::make_shared<D3DManager>();
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	/* Shader Manager Class */
	m_ShaderManager = std::make_shared<ShaderManager>();
	if (!m_ShaderManager)
	{
		return false;
	}

	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	/* Texture Manager Class */
	m_TextureManager = std::make_shared<TextureManager>(m_Direct3D->GetDevice());
	if (!m_TextureManager)
	{
		return false;
	}
	
	result = m_TextureManager->Initialize(10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture manager object.", L"Error", MB_OK);
		return false;
	}
	
	if (!m_TextureManager->LoadTexture("silver_surfer_GLOSS.png", 0)) { return false; }
	if (!m_TextureManager->LoadTexture("silver_surfer_NORM.png", 1)) { return false; }
	if (!m_TextureManager->LoadTexture("rock01d.tga", 2)) { return false; }
	if (!m_TextureManager->LoadTexture("rock01n.tga", 3)) { return false; }
	if (!m_TextureManager->LoadTexture("sphere_bump.jpg", 4)) { return false; }


	/* Timer Class */
	m_Timer = std::make_shared<Timer>();
	if (!m_Timer)
	{
		return false;
	}

	/* FPS Class */
	m_Fps = std::make_shared<FramesPerSec>();
	if (!m_Fps)
	{
		return false;
	}

	/* Scene Class */
	m_Scene = std::make_shared<Scene>();
	if (!m_Scene)
	{
		return false;
	}

	result = m_Scene->Initialize(m_Direct3D.get(), hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Scene object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	if (m_Scene)
	{
		m_Scene->Shutdown();
		m_Scene.reset();
		m_Scene = nullptr;
	}

	if (m_Fps)
	{
		m_Fps.reset();
		m_Fps = nullptr;
	}

	if (m_Timer)
	{
		m_Timer.reset();
		m_Timer = nullptr;
	}

	if (m_TextureManager)
	{
		m_TextureManager->Shutdown();
		m_TextureManager.reset();
		m_TextureManager = nullptr;
	}

	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		m_ShaderManager.reset();
		m_ShaderManager = nullptr;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		m_Input.reset();
		m_Input = nullptr;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		m_Direct3D.reset();
		m_Direct3D = nullptr;
	}

	return;
}

bool Application::Frame()
{
	bool result;

	
	m_Fps->Frame();
	m_Timer->Frame();

	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}

	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	result = m_Scene->Frame(m_Direct3D.get(), m_Input.get(), m_ShaderManager.get(), m_TextureManager.get(), m_Timer->GetTime(), m_Fps->GetFps());
	if (!result)
	{
		return false;
	}

	return result;
}