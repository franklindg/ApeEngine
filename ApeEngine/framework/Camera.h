#ifndef _CAMERA_H
#define _CAMERA_H

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(DirectX::XMMATRIX&);

	void RenderReflection(float);
	void GetReflectionViewMatrix(DirectX::XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::XMMATRIX m_viewMatrix, m_baseViewMatrix, m_reflectionViewMatrix;
};
#endif