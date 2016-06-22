/////////////////////////////////////////////
// Filename: UserInterface.cpp
/////////////////////////////////////////////
#include "UserInterface.h"

UserInterface::UserInterface()
	: m_pFont(nullptr)
	, m_pFpsString(nullptr)
	, m_pVideoStrings(nullptr)
	, m_pPositionStrings(nullptr)
	, m_pRenderCountStrings(nullptr)
	, m_pMouseCursor(nullptr)
{}


UserInterface::UserInterface(const UserInterface& other)
{
}


UserInterface::~UserInterface()
{
}

bool UserInterface::Initialize(D3DManager* Direct3D, int screenHeight, int screenWidth)
{
	bool result;
	char videoCard[128];
	int videoMemory;
	char videoString[144];
	char memoryString[32];
	char tempString[16];
	int i;


	// Create the first font object.
	m_pFont = new apeFont;
	if (!m_pFont)
	{
		return false;
	}

	// Initialize the first font object.
	result = m_pFont->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "data/userinterface/font/font01.txt",
		"font01.tga", 32.0f, 3);
	if (!result)
	{
		return false;
	}

	// Create the text object for the fps string.
	m_pFpsString = new Text;
	if (!m_pFpsString)
	{
		return false;
	}

	// Initialize the fps text string.
	result = m_pFpsString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"Fps: 0", 10, 50, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	// Initial the previous frame fps.
	m_previousFps = -1;

	// Setup the video card strings.
	Direct3D->GetVideoCardInfo(videoCard, videoMemory);
	strcpy_s(videoString, "Video Card: ");
	strcat_s(videoString, videoCard);

	_itoa_s(videoMemory, tempString, 10);

	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Create the text objects for the video strings.
	m_pVideoStrings = new Text[2];
	if (!m_pVideoStrings)
	{
		return false;
	}

	// Initialize the video text strings.
	result = m_pVideoStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, false, m_pFont,
		videoString, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pVideoStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_pFont,
		memoryString, 10, 30, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Create the text objects for the position strings.
	m_pPositionStrings = new Text[6];
	if (!m_pPositionStrings)
	{
		return false;
	}

	// Initialize the position text strings.
	result = m_pPositionStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"X: 0", 10, 310, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pPositionStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"Y: 0", 10, 330, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pPositionStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"Z: 0", 10, 350, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pPositionStrings[3].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"rX: 0", 10, 370, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pPositionStrings[4].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"rY: 0", 10, 390, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pPositionStrings[5].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_pFont,
		"rZ: 0", 10, 410, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Initialize the previous frame position.
	for (i = 0; i<6; i++)
	{
		m_previousPosition[i] = -1;
	}

	m_pRenderCountStrings = new Text[3];
	if (!m_pRenderCountStrings)
	{
		return false;
	}

	result = m_pRenderCountStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false,
		m_pFont, "Polys Drawn: 0", 10, 260, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pRenderCountStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_pFont,
		"Cells Drawn: 0", 10, 280, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_pRenderCountStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_pFont,
		"Cells Culled: 0", 10, 300, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_pMouseCursor = new MouseCursor;
	if (!m_pMouseCursor)
	{
		return false;
	}

	result = m_pMouseCursor->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	return true;
}

void UserInterface::Shutdown()
{
	if (m_pMouseCursor)
	{
		m_pMouseCursor->Shutdown();
		delete m_pMouseCursor;
		m_pMouseCursor = nullptr;
	}

	if (m_pRenderCountStrings)
	{
		m_pRenderCountStrings[0].Shutdown();
		m_pRenderCountStrings[1].Shutdown();
		m_pRenderCountStrings[2].Shutdown();

		delete[] m_pRenderCountStrings;
		m_pRenderCountStrings = nullptr;
	}

	// Release the position text strings.
	if (m_pPositionStrings)
	{
		m_pPositionStrings[0].Shutdown();
		m_pPositionStrings[1].Shutdown();
		m_pPositionStrings[2].Shutdown();
		m_pPositionStrings[3].Shutdown();
		m_pPositionStrings[4].Shutdown();
		m_pPositionStrings[5].Shutdown();

		delete[] m_pPositionStrings;
		m_pPositionStrings = nullptr;
	}

	// Release the video card string.
	if (m_pVideoStrings)
	{
		m_pVideoStrings[0].Shutdown();
		m_pVideoStrings[1].Shutdown();

		delete[] m_pVideoStrings;
		m_pVideoStrings = nullptr;
	}


	// Release the fps text string.
	if (m_pFpsString)
	{
		m_pFpsString->Shutdown();
		delete m_pFpsString;
		m_pFpsString = nullptr;
	}

	// Release the font object.
	if (m_pFont)
	{
		m_pFont->Shutdown();
		delete m_pFont;
		m_pFont = nullptr;
	}

	return;
}

bool UserInterface::Frame(ID3D11DeviceContext* deviceContext, Input* input, int fps, float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ)
{
	bool result;


	// Update the fps string.
	result = UpdateFpsString(deviceContext, fps);
	if (!result)
	{
		return false;
	}

	// Update the position strings.
	result = UpdatePositionStrings(deviceContext, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	m_pMouseCursor->PositionUpdate(input);
	return true;
}


bool UserInterface::Render(D3DManager* Direct3D, ShaderManager* ShaderManager, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
	CXMMATRIX orthoMatrix)
{
	int i;
	bool result;


	Direct3D->TurnZBufferOff();
	Direct3D->TurnAlphaBlendingOn();


	m_pFpsString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_pFont->GetTexture());

	m_pVideoStrings[0].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_pFont->GetTexture());
	m_pVideoStrings[1].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_pFont->GetTexture());

	for (i = 0; i<6; i++)
	{
		m_pPositionStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_pFont->GetTexture());
	}

	for (i = 0; i < 3; i++)
	{
		m_pRenderCountStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_pFont->GetTexture());
	}


	result = m_pMouseCursor->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}


	Direct3D->TurnAlphaBlendingOff();

	// Turn the Z buffer back on now that the 2D rendering has completed.
	Direct3D->TurnZBufferOn();

	return true;
}


bool UserInterface::UpdateFpsString(ID3D11DeviceContext* deviceContext, int fps)
{
	char tempString[16];
	char finalString[16];
	float red, green, blue;
	bool result;


	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_pFpsString->UpdateSentence(deviceContext, m_pFont, finalString, 10, 50, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}


bool UserInterface::UpdatePositionStrings(ID3D11DeviceContext* deviceContext, float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ)
{
	int positionX, positionY, positionZ, rotationX, rotationY, rotationZ;
	char tempString[16];
	char finalString[16];
	bool result;


	// Convert the float values to integers.
	positionX = (int)posX;
	positionY = (int)posY;
	positionZ = (int)posZ;
	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	// Update the position strings if the value has changed since the last frame.
	if (positionX != m_previousPosition[0])
	{
		m_previousPosition[0] = positionX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[0].UpdateSentence(deviceContext, m_pFont, finalString, 10, 100, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	if (positionY != m_previousPosition[1])
	{
		m_previousPosition[1] = positionY;
		_itoa_s(positionY, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[1].UpdateSentence(deviceContext, m_pFont, finalString, 10, 120, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	if (positionZ != m_previousPosition[2])
	{
		m_previousPosition[2] = positionZ;
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[2].UpdateSentence(deviceContext, m_pFont, finalString, 10, 140, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	if (rotationX != m_previousPosition[3])
	{
		m_previousPosition[3] = rotationX;
		_itoa_s(rotationX, tempString, 10);
		strcpy_s(finalString, "rX: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[3].UpdateSentence(deviceContext, m_pFont, finalString, 10, 180, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	if (rotationY != m_previousPosition[4])
	{
		m_previousPosition[4] = rotationY;
		_itoa_s(rotationY, tempString, 10);
		strcpy_s(finalString, "rY: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[4].UpdateSentence(deviceContext, m_pFont, finalString, 10, 200, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	if (rotationZ != m_previousPosition[5])
	{
		m_previousPosition[5] = rotationZ;
		_itoa_s(rotationZ, tempString, 10);
		strcpy_s(finalString, "rZ: ");
		strcat_s(finalString, tempString);
		result = m_pPositionStrings[5].UpdateSentence(deviceContext, m_pFont, finalString, 10, 220, 0.5f, 0.9f, 0.0f);
		if (!result) { return false; }
	}

	return true;
}

bool UserInterface::UpdateRenderCounts(ID3D11DeviceContext* deviceContext, int renderCount, int nodesDrawn, int nodesCulled)
{
	char tempString[32];
	char finalString[32];
	bool result;

	// Convert the render count integer to string format.
	_itoa_s(renderCount, tempString, 10);

	// Setup the render count string.
	strcpy_s(finalString, "Polys Drawn: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_pRenderCountStrings[0].UpdateSentence(deviceContext, m_pFont, finalString, 10, 260, 0.5f, 0.9f, 0.0f);
	if (!result)
	{
		return false;
	}

	// Convert the cells drawn integer to string format.
	_itoa_s(nodesDrawn, tempString, 10);

	// Setup the cells drawn string.
	strcpy_s(finalString, "Cells Drawn: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_pRenderCountStrings[1].UpdateSentence(deviceContext, m_pFont, finalString, 10, 280, 0.5f, 0.9f, 0.0f);
	if (!result)
	{
		return false;
	}

	// Convert the cells culled integer to string format.
	_itoa_s(nodesCulled, tempString, 10);

	// Setup the cells culled string.
	strcpy_s(finalString, "Cells Culled: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_pRenderCountStrings[2].UpdateSentence(deviceContext, m_pFont, finalString, 10, 300, 0.5f, 0.9f, 0.0f);
	if (!result)
	{
		return false;
	}

	return true;
}