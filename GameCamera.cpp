#include "GameCamera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 800.0f;
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

	//カメラ座標を設定
	m_cameraPos = m_playerPos + kInitPos;
	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

}

void GameCamera::Update()
{
	////移動量
	//MyEngine::Vector3 velo = m_playerVelo;

	////移動ベクトルを現在の座標に足す
	//m_cameraPos += velo;

	MyEngine::Vector3 shiftVec =  VTransform(kInitPos.CastVECTOR(),m_playerRotaMat);

	MyEngine::Vector3 pos = shiftVec;

	m_cameraPos = m_playerPos + pos;

	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	//カメラの座標を設定

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
