#include "Camera.h"

namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 1.5f;
}

Camera::Camera()
{
	SetCameraNearFar(1.0f,10000.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	m_cameraPos = m_playerPos/* - (m_playerPos - m_targetPos)*/;


	MyEngine::Vector3 dir;

	dir = m_cameraPos - m_targetPos;

	dir = dir.Normalize();

	SetCameraPositionAndAngle(m_cameraPos.CastVECTOR(),dir.x,dir.y,dir.z);
}
