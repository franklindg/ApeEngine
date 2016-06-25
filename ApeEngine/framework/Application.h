#ifndef _APPLICATION_H
#define _APPLICATION_H

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Input;
class D3DManager;
class Texture;
class Timer;
class FramesPerSec;
class Scene;

class Application
{
public:
	Application(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Frame();

private:
	std::shared_ptr<Input> m_Input;
	std::shared_ptr<D3DManager> m_Direct3D;
	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Timer> m_Timer;
	std::shared_ptr<FramesPerSec> m_Fps;
	std::shared_ptr<Scene> m_Scene;
};
#endif
