#include "GameCamera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 2000.0f;
	//カメラの移動速度(距離の倍率)
	constexpr float kCameraSpeed = 0.1f;
	//カメラの高さ(プレイヤーから見て)
	constexpr float kCameraHeight = 1000.0f;
	//回転をしない猶予範囲
	constexpr float kRotaMargin = 600.0f;
}

GameCamera::GameCamera() :
	m_rota(0),
	m_playerRota(0)
{
	SetCameraNearFar(10.0f, 100000.0f);
}

GameCamera::~GameCamera()
{
}

void GameCamera::Init()
{
	m_cameraPos = MyEngine::Vector3(300,500,-500);
}

void GameCamera::Update()
{
	//移動量
	MyEngine::Vector3 velo;

	//エネミーから見てプレイヤーの角度を参照し座標を決定する
	//回転の中心とする座標
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//回転の中心からプレイヤーに向かうベクトルを作成し
	//プレイヤーの後ろにカメラを持ってくる
	MyEngine::Vector3 centerToPlayer = (m_playerPos - rotationShaftPos);

	rotationShaftPos.y = m_playerPos.y + kCameraHeight;
	//次に向かう座標
	MyEngine::Vector3 pos;
	//回転の中心から今の座標を計算する
	pos.x = rotationShaftPos.x + cosf(m_rota) * centerToPlayer.Length();
	pos.y = rotationShaftPos.y;
	pos.z = rotationShaftPos.z + sinf(m_rota) * centerToPlayer.Length();

	//中心からプレイヤーの距離によってカメラの距離を変える
	pos += centerToPlayer.Normalize() * kPlayerDistance;
	//次の座標に向かう移動ベクトルを作成
	velo = pos - m_cameraPos;
	//移動ベクトルを現在の座標に足す
	m_cameraPos += velo;
	//カメラのターゲット座標を作成
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;
	
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

	float margin = kRotaMargin / centerToPlayer.Length();
	//プレイヤーの真後ろをずっとカメラがついていかないように
	//動きに猶予を持たせる
	if (m_rota > m_playerRota + margin)
	{
		m_rota -= m_rota - m_playerRota - margin;
	}
	else if (m_rota < m_playerRota - margin)
	{
		m_rota += m_playerRota - m_rota - margin;
	}
}
