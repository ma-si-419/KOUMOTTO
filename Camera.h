#pragma once
#include "Vector3.h"
class Camera
{
public:
	Camera();
	~Camera();

	void Init();

	void Update();

	void SetPlayerPos(MyEngine::Vector3 pos) { m_playerPos = pos; }

	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }

private:

	//カメラの座標
	MyEngine::Vector3 m_cameraPos;
	//プレイヤーの座標
	MyEngine::Vector3 m_playerPos;
	//ターゲットの座標
	MyEngine::Vector3 m_targetPos;

};

