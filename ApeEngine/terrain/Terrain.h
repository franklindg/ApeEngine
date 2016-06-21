/////////////////////////////////////////////
// Filename: Terrain.h
/////////////////////////////////////////////
#ifndef _TERRAIN_H
#define _TERRAIN_H


//////////
// INCLUDES //
//////////
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <fstream>
#include <stdio.h>

//////////
// CLASS INCLUDES //
//////////
#include "TerrainCells.h"
#include "../utilitiesfrw/Frustrum.h"

using namespace DirectX;
using namespace std;


///////////////////
// Class name: Terrain //
///////////////////
class Terrain
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
		XMFLOAT3 color;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
		float r, g, b;
		float tu2, tv2;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*, char*);
	void Shutdown();

	void Frame();
	bool RenderCell(ID3D11DeviceContext*, int, Frustrum*);
	void RenderCellLines(ID3D11DeviceContext*, int);

	int GetCellIndexCount(int);
	int GetCellLinesIndexCount(int);
	int GetCellCount();

	int GetRenderCount();
	int GetCellsDrawn();
	int GetCellsCulled();

	bool GetHeightAtPosition(float, float, float&);

private:
	bool LoadSetupFile(char*);
	bool LoadRawHeightMap();
	bool LoadBitmapHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();
	bool LoadColorMap();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();
	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	bool LoadTerrainCells(ID3D11Device*);
	void ShutdownTerrainCells();
	bool CheckHeightOfTriangle(float, float, float&, float[3], float[3], float[3]);

private:
	int m_terrainHeight, m_terrainWidth, m_vertexCount;
	float m_heightScale;
	char *m_terrainFileName, *m_colorMapFilename;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;
	TerrainCells* m_TerrainCells;
	int m_cellCount, m_renderCount, m_cellsDrawn, m_cellsCulled;

};

#endif