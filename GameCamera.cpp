#include "GameCamera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 2400.0f;
	//�J�����̈ړ����x(�����̔{��)
	constexpr float kCameraSpeed = 0.1f;
	//�J�����̍���(�v���C���[���猩��)
	constexpr float kCameraHeight = 1000.0f;
	//��]�����Ȃ��P�\�͈�
	constexpr float kRotaMargin = 600.0f;
	//�J�����̏����ʒu
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

	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) / 2;

	//�����_�̕\���̂���
	m_playerVelo = cameraTarget;

	//�J�����̍��W��ݒ�

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}

void GameCamera::Update()
{
	////�ړ���
	//MyEngine::Vector3 velo = m_playerVelo;

	////�ړ��x�N�g�������݂̍��W�ɑ���
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

	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) / 2;

	//�����_�̕\���̂���
	m_playerVelo = cameraTarget;

	//�J�����̍��W��ݒ�

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
