/////////////////////////////////////////////
// Filename: MiniMap.h
/////////////////////////////////////////////
#ifndef _MINIMAP_H
#define _MINIMAP_H


//////////
// CLASS INCLUDES //
//////////
#include "../render2d/Bitmap.h"
#include "../framework/ShaderManager.h"

using namespace DirectX;
using namespace std;


///////////////////
// Class name: MiniMap //
///////////////////
class MiniMap
{
public:
	MiniMap();
	MiniMap(const MiniMap&);
	~MiniMap();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, float, float);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, ShaderManager*, CXMMATRIX, CXMMATRIX, CXMMATRIX);

	void PositionUpdate(float, float);

private:
	int m_mapLocationX, m_mapLocationY, m_pointLocationX, m_pointLocationY;
	float m_mapSizeX, m_mapSizeY, m_terrainWidth, m_terrainHeight;
	Bitmap *m_MiniMapBitmap, *m_PointBitmap;
};

#endif