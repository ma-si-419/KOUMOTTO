#pragma once
#include "Vector3.h"
class GameCamera
{
public:
	GameCamera();
	~GameCamera();

	void Init();

	void Update();

	MyEngine::Vector3 GetPos() { return m_cameraPos; }

	void SetPlayerPos(MyEngine::Vector3 pos) { m_playerPos = pos; }

	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }

	//void SetPlayerVelo(MyEngine::Vector3 velo) { m_playerVelo = velo; }

	void SetPlayerRotaMat(MATRIX mat) { m_playerRotaMat = mat; }
private:

	//カメラの座標
	MyEngine::Vector3 m_cameraPos;
	//プレイヤーの座標
	MyEngine::Vector3 m_playerPos;
	//ターゲットの座標
	MyEngine::Vector3 m_targetPos;
	//プレイヤーの移動ベクトル
	MyEngine::Vector3 m_playerVelo;
	//プレイヤーの回転行列
	MATRIX m_playerRotaMat;
	//プレイヤーがターゲットを中心にどのくらい回転しているのか
	float m_playerRota;

};

