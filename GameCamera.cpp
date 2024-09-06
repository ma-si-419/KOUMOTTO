#include "GameCamera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 2400.0f;
	//カメラの移動速度(距離の倍率)
	constexpr float kCameraSpeed = 0.1f;
	//カメラの高さ(プレイヤーから見て)
	constexpr float kCameraHeight = 1000.0f;
	//回転をしない猶予範囲
	constexpr float kRotaMargin = 600.0f;
	//カメラの初期位置
	MyEngine::Vector3 kInitPos(0,-5000,-10000);
}

GameCamera::GameCamera() :
	m_playerRota(0)
{
	SetCameraNearFar(10.0f, 100000.0f);
}

GameCamera::~GameCamera()
{
}

void GameCamera::Init()
{

	m_playerPos.y += 1000;

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
}

void GameCamera::Update()
{
	////移動量
	//MyEngine::Vector3 velo = m_playerVelo;

	////移動ベクトルを現在の座標に足す
	//m_cameraPos += velo;

	m_playerPos.y += 1000;

	MyEngine::Vector3 enemyToPlayer = m_playerPos - m_targetPos;

	float vecSize = enemyToPlayer.Length();

	MyEngine::Vector3 unitVec = enemyToPlayer.Normalize();

	m_cameraPos.x = enemyToPlayer.x + kPlayerDistance * unitVec.x;
	m_cameraPos.y = enemyToPlayer.y + kPlayerDistance * unitVec.y;
	m_cameraPos.z = enemyToPlayer.z + kPlayerDistance * unitVec.z;

	MATRIX mat = MGetRotY(0.5 / (vecSize* 0.001));

	m_cameraPos = m_cameraPos.MatTransform(mat);
	
	m_cameraPos += m_targetPos;

	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) / 2;

	//注視点の表示のため
	m_playerVelo = cameraTarget;

	//カメラの座標を設定

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
