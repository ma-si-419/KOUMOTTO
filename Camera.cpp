#include "Camera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 1.0f;
	//デフォルトのカメラの位置
	const MyEngine::Vector3 kDefaultPos(500, 500, -5000);
	//カメラの移動速度(距離の倍率)
	constexpr float kCameraSpeed = 0.1f;
}

Camera::Camera()
{
	SetCameraNearFar(1.0f, 10000.0f);
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_cameraPos = m_playerPos + kDefaultPos;
}

void Camera::Update()
{
	//エネミーからプレイヤーへのベクトル作成
	MyEngine::Vector3 targetToPlayerVec = m_playerPos - m_targetPos;
	
	//X軸回転させる
	MATRIX x = MGetRotX(-10);
	//Y軸回転させる
	MATRIX y = MGetRotY(10);

	MATRIX m = MMult(x, y);
	MyEngine::Vector3 moveVec = VTransformSR(targetToPlayerVec.CastVECTOR(),m);

	m_cameraPos = m_playerPos - moveVec * kPlayerDistance;


	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
