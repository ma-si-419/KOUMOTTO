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

	//�J�����̍��W
	MyEngine::Vector3 m_cameraPos;
	//�v���C���[�̍��W
	MyEngine::Vector3 m_playerPos;
	//�^�[�Q�b�g�̍��W
	MyEngine::Vector3 m_targetPos;

};

