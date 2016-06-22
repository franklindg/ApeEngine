/////////////////////////////////////////////
// Filename: Application.cpp
/////////////////////////////////////////////
#include "Application.h"

Application::Application(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
	: m_Input(nullptr)
	, m_Direct3D(nullptr)
	, m_Timer(nullptr)
	, m_Fps(nullptr)
	, m_Texture(nullptr)
	, m_ShaderManager(nullptr)
	, m_Scene(nullptr)
{
	m_Input = std::make_shared<Input>(hinstance, hwnd, screenWidth, screenHeight);
	m_Direct3D = std::make_shared<D3DManager>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	m_ShaderManager = std::make_shared<ShaderManager>(m_Direct3D->GetDevice(), hwnd);
	m_Texture = std::make_shared<Texture>(m_Direct3D->GetDevice());
	m_Timer = std::make_shared<Timer>();
	m_Fps = std::make_shared<FramesPerSec>();
	m_Scene = std::make_shared<Scene>(m_Direct3D.get(), hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);

	m_Texture->LoadTexture("rock01d.tga");
	m_Texture->LoadTexture("rock01n.tga");
}

Application::~Application()
{
	m_Scene.reset();
	m_Fps.reset();
	m_Timer.reset();
	m_ShaderManager.reset();
	m_Input.reset();
	m_Direct3D.reset();
}

bool Application::Frame()
{	
	m_Fps->Frame();
	m_Timer->Frame();

	if(!m_Input->Frame())
		return false;

	if (m_Input->IsEscapePressed() == true)
		return false;

	if (!m_Scene->Frame(m_Direct3D.get(), m_Input.get(), m_ShaderManager.get(), m_Texture.get(), m_Timer->GetTime(), m_Fps->GetFps()))
		return false;

	return true;
}