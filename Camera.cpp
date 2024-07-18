#include "Camera.h"
#include <cmath>
namespace
{
	//カメラとプレイヤーの距離
	constexpr float kPlayerDistance = 1500.0f;
	//カメラの移動速度(距離の倍率)
	constexpr float kCameraSpeed = 0.1f;
	//カメラの高さ(プレイヤーから見て)
	constexpr float kCameraHeight = 1500.0f;
	//回転をしない猶予範囲
	constexpr float kRotaMargin = 0.3f;
}

Camera::Camera() :
	m_rota(0),
	m_playerRota(0)
{
	SetCameraNearFar(10.0f, 30000.0f);
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_cameraPos = m_playerPos;
}

void Camera::Update()
{
	//プレイヤーの回転と完全に一致しないようにカメラが動かない猶予を持たせる
	if (m_rota > m_playerRota + kRotaMargin)
	{
		m_rota -= m_rota - m_playerRota - kRotaMargin;
	}
	else if (m_rota < m_playerRota - kRotaMargin)
	{
		m_rota += m_playerRota - m_rota - kRotaMargin;
	}

	DrawFormatString(500, 0, GetColor(255, 255, 255), "cameraRota:%f", m_rota);
	DrawFormatString(700, 0, GetColor(255, 255, 255), "playerRota:%f", m_playerRota);

	//移動量
	MyEngine::Vector3 velo;

	//エネミーから見てプレイヤーの角度を参照し座標を決定する
	//回転の中心とする座標
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	MyEngine::Vector3 centerToPlayer = (m_playerPos - rotationShaftPos);
	//回転の中心からプレイヤーに向かうベクトルを作成し
	//プレイヤーの後ろにカメラを持ってくる

	rotationShaftPos.y = m_playerPos.y + kCameraHeight;

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * centerToPlayer.Length();
	pos.y = rotationShaftPos.y;
	pos.z = rotationShaftPos.z + sinf(m_rota) * centerToPlayer.Length();

	pos += centerToPlayer.Normalize() * kPlayerDistance;

	velo = pos - m_cameraPos;

	m_cameraPos += velo;

	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
