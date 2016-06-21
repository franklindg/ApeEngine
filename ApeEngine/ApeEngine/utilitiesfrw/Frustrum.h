/////////////////////////////////////////////
// Filename: Frustum.h
/////////////////////////////////////////////
#ifndef _FRUSTUM_H
#define _FRUSTUM_H


//////////
// INCLUDES //
//////////
#include <DirectXMath.h>
using namespace DirectX;


///////////////////
// Class name: Frustrum //
///////////////////
class Frustrum
{
public:
	Frustrum();
	Frustrum(const Frustrum&);
	~Frustrum();

	void Initialize(float);

	void ConstructFrustrum(XMFLOAT4X4, XMFLOAT4X4);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);
	bool CheckRectangle2(float, float, float, float, float, float);

private:
	float m_screenDepth;
	float m_planes[6][4];
};

#endif