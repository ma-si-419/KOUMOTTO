#include "GameCamera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 2400.0f;
	//カメラの移動速度(距離の倍率)
	constexpr float kCameraSpeed = 0.1f;
	//カメラの高さ
	constexpr float kCameraHeight = 1000.0f;
	//回転をしない猶予範囲
	constexpr float kRotaMargin = 600.0f;
	//カメラの初期位置
	MyEngine::Vector3 kInitPos(0, -5000, -10000);
	//カメラの回転具合を距離によって回していきます
	constexpr float kDistanceRotaYDecayRate = 0.001f;
	//カメラの初期視野角(60度)
	constexpr float kInitCameraFov = 1.046f;
	//カメラを広げた時の視野角(80度)
	constexpr float kWideCameraFov = 1.395f;
	//カメラの視野角を拡縮する速度
	constexpr float kFovScalingSpeed = (kWideCameraFov - kInitCameraFov) * 0.05f;
	//カメラをあげるときの上昇速度
	constexpr float kCameraUpSpeed = 10.0f;
	//カメラの最大上昇値
	constexpr float kCameraMaxUpPos = 100.0f;
	//スカイドームの大きさ
	constexpr float kSkyDomeScale = 600;
}

GameCamera::GameCamera() :
	m_playerRota(0),
	m_fov(kInitCameraFov),
	m_cameraUpPos(0)
{
	SetCameraNearFar(10.0f, 100000.0f);
	m_domeHandle = MV1LoadModel("data/model/Dome.mv1");
	MV1SetScale(m_domeHandle, VGet(kSkyDomeScale, kSkyDomeScale, kSkyDomeScale));
}

GameCamera::~GameCamera()
{
	MV1DeleteModel(m_domeHandle);
}

void GameCamera::Init()
{
	//ライトの作成
	m_lightHandle = CreateDirLightHandle(VGet(0, 0, 1));

	m_playerPos.y += kCameraHeight;

	MyEngine::Vector3 enemyToPlayer = m_playerPos - m_targetPos;

	float vecSize = enemyToPlayer.Length();

	MyEngine::Vector3 unitVec = enemyToPlayer.Normalize();

	m_cameraPos.x = enemyToPlayer.x + kPlayerDistance * unitVec.x;
	m_cameraPos.y = enemyToPlayer.y + kPlayerDistance * unitVec.y;
	m_cameraPos.z = enemyToPlayer.z + kPlayerDistance * unitVec.z;

	MATRIX mat = MGetRotY(0.5 / (vecSize * kDistanceRotaYDecayRate));

	m_cameraPos = m_cameraPos.MatTransform(mat);

	m_cameraPos += m_targetPos;

	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) / 2;

	//注視点の表示のため
	m_playerVelo = cameraTarget;

	//カメラの座標を設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

}

void GameCamera::Update()
{

	if (m_isUpFov)
	{
		m_fov += kFovScalingSpeed;
		m_cameraUpPos += kCameraUpSpeed;

		if (m_fov > kWideCameraFov)
		{
			m_fov = kWideCameraFov;
		}
		if (m_cameraUpPos > kCameraMaxUpPos)
		{
			m_cameraUpPos = kCameraMaxUpPos;
		}
	}
	else
	{
		m_fov -= kFovScalingSpeed;
		m_cameraUpPos -= kCameraUpSpeed;
		if (m_fov < kInitCameraFov)
		{
			m_fov = kInitCameraFov;
		}
		if (m_cameraUpPos < 0)
		{
			m_cameraUpPos = 0;
		}
	}

	SetupCamera_Perspective(m_fov);

	m_playerPos.y += 500 + m_cameraUpPos;

	MyEngine::Vector3 enemyToPlayer = m_playerPos - m_targetPos;

	float vecSize = enemyToPlayer.Length();

	MyEngine::Vector3 unitVec = enemyToPlayer.Normalize();

	m_cameraPos.x = enemyToPlayer.x + kPlayerDistance * unitVec.x;
	m_cameraPos.y = enemyToPlayer.y + kPlayerDistance * unitVec.y;
	m_cameraPos.z = enemyToPlayer.z + kPlayerDistance * unitVec.z;

	MATRIX mat = MGetRotY(0.5 / (vecSize * 0.001));

	m_cameraPos = m_cameraPos.MatTransform(mat);

	m_cameraPos += m_targetPos;

	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) / 2;

	//注視点の表示のため
	m_playerVelo = cameraTarget;

	//カメラの座標を設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
	//視野角を広げるかどうかをfalseにする
	m_isUpFov = false;
	//カメラの座標にスカイドームを設定
	MV1SetPosition(m_domeHandle, m_cameraPos.CastVECTOR());

	//カメラの見ている方向にディレクションライトを設定
	SetLightDirectionHandle(m_lightHandle, (m_targetPos - m_cameraPos).Normalize().CastVECTOR());
}