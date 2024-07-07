#include "Camera.h"

namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 2.5f;
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
	//m_cameraPos = m_targetPos;
	m_cameraPos = m_playerPos;

	/*MyEngine::Vector3 targetToPlayer = m_playerPos - m_targetPos;

	m_cameraPos = m_cameraPos + targetToPlayer.Normalize() * 1000;*/

	m_cameraPos.y = m_cameraPos.y + 500;
	m_cameraPos.z = m_cameraPos.z - 5000;


	MyEngine::Vector3 target = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(),target.CastVECTOR());
}
