#include <ApePCH.h>
#include "Camera.h"

Camera::Camera()
	: m_positionX(0.0f)
	, m_positionY(0.0f)
	, m_positionZ(0.0f)
	, m_rotationX(0.0f)
	, m_rotationY(0.0f)
	, m_rotationZ(0.0f)
{}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;


	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// Sets the camera position in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// Sets default camera direction.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// Sets the pitch, yaw, roll to radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	return;
}

void Camera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void Camera::RenderBaseViewMatrix()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = m_positionX;
	position.y = m_positionY;	
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	m_baseViewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::GetBaseViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_baseViewMatrix;
	return;
}

void Camera::RenderReflection(float height)
{
	DirectX::XMFLOAT3 up, position, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float radians;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVector = XMLoadFloat3(&up);

	position.x = m_positionX;
	position.y = -m_positionY + (height * 2.0f);
	position.z = m_positionZ;
	positionVector = XMLoadFloat3(&position);

	radians = m_rotationY * 0.0174532925f;

	lookAt.x = sinf(radians) + m_positionX;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_positionZ;
	lookAtVector = XMLoadFloat3(&lookAt);

	m_reflectionViewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::GetReflectionViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_reflectionViewMatrix;
	return;
}