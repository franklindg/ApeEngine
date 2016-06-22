#ifndef _APPLICATION_H
#define _APPLICATION_H

// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// CLASS INCLUDES
#include "ApePCH.h"
#include "Input.h"
#include "D3DManager.h"
#include "ShaderManager.h"
#include "../utilities/Timer.h"
#include "../utilities/FramesPerSec.h"
#include "Scene.h"
#include "TextureManager.h"


///////////////////
// Class name: Application //
///////////////////
class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	std::shared_ptr<Input> m_Input;
	std::shared_ptr<D3DManager> m_Direct3D;
	std::shared_ptr<ShaderManager> m_ShaderManager;
	std::shared_ptr<TextureManager> m_TextureManager;
	std::shared_ptr<Timer> m_Timer;
	std::shared_ptr<FramesPerSec> m_Fps;
	std::shared_ptr<Scene> m_Scene;
};
#endif
