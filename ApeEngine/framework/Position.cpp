#include <ApePCH.h>
#include "Position.h"

Position::Position()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_leftMoveSpeed = 0.0f;
	m_rightMoveSpeed = 0.0f;
	m_upwardSpeed = 0.0f;
	m_downwardSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;
}

Position::~Position()
{
}

void Position::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Position::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void Position::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

void Position::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

void Position::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void Position::MoveForward(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 2.4f;
		if (m_forwardSpeed > (m_frameTime * 40.0f))
		{
			m_forwardSpeed = m_frameTime * 40.0f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 2.4f;
		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}

void Position::MoveBackward(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 2.4f;
		if (m_backwardSpeed > (m_frameTime * 40.0f))
		{
			m_backwardSpeed = m_frameTime * 40.0f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 2.4f;
		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}

void Position::MoveRight(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_rightMoveSpeed += m_frameTime * 3.2f;
		if (m_rightMoveSpeed > (m_frameTime * 40.0f))
		{
			m_rightMoveSpeed = m_frameTime * 40.0f;
		}
	}
	else
	{
		m_rightMoveSpeed -= m_frameTime * 3.2f;
		if (m_rightMoveSpeed < 0.0f)
		{
			m_rightMoveSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX += cosf(radians) * m_rightMoveSpeed;
	m_positionZ -= sinf(radians) * m_rightMoveSpeed;

	return;
}

void Position::MoveLeft(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_leftMoveSpeed += m_frameTime * 3.2f;
		if (m_leftMoveSpeed > (m_frameTime * 40.0f))
		{
			m_leftMoveSpeed = m_frameTime * 40.0f;
		}
	}
	else
	{
		m_leftMoveSpeed -= m_frameTime * 3.2f;
		if (m_leftMoveSpeed < 0.0f)
		{
			m_leftMoveSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX -= cosf(radians) * m_leftMoveSpeed;
	m_positionZ += sinf(radians) * m_leftMoveSpeed;

	return;
}

void Position::MoveUpward(bool keydown)
{
	if (keydown)
	{
		m_upwardSpeed += m_frameTime * 2.0f;

		if (m_upwardSpeed > (m_frameTime * 10.0f))
		{
			m_upwardSpeed = m_frameTime * 10.0f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 2.0f;

		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	m_positionY += m_upwardSpeed;

	return;
}

void Position::MoveDownward(bool keydown)
{
	if (keydown)
	{
		m_downwardSpeed += m_frameTime * 2.0f;

		if (m_downwardSpeed > (m_frameTime * 10.0f))
		{
			m_downwardSpeed = m_frameTime * 10.0f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 2.0f;

		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	m_positionY -= m_downwardSpeed;

	return;
}

void Position::TurnLeft(bool keydown)
{
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 20.0f;

		if (m_leftTurnSpeed > (m_frameTime * 70.0f))
		{
			m_leftTurnSpeed = m_frameTime * 70.0f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 20.0f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotationY -= m_leftTurnSpeed;

	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}

void Position::TurnRight(bool keydown)
{
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 20.0f;

		if (m_rightTurnSpeed > (m_frameTime * 70.0f))
		{
			m_rightTurnSpeed = m_frameTime * 70.0f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 20.0f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;

	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}

void Position::LookUpward(bool keydown)
{
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 60.0f;

		if (m_lookUpSpeed > (m_frameTime * 60.0f))
		{
			m_lookUpSpeed = m_frameTime * 60.0f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime * 60.0f;

		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	m_rotationX -= m_lookUpSpeed;

	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}

void Position::LookDownward(bool keydown)
{
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 60.0f;

		if (m_lookDownSpeed > (m_frameTime * 60.0f))
		{
			m_lookDownSpeed = m_frameTime * 60.0f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 60.0f;

		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	m_rotationX += m_lookDownSpeed;

	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}