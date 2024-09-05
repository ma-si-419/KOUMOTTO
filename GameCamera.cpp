#include "GameCamera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 800.0f;
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

	//�J�������W��ݒ�
	m_cameraPos = m_playerPos + kInitPos;
	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

}

void GameCamera::Update()
{
	////�ړ���
	//MyEngine::Vector3 velo = m_playerVelo;

	////�ړ��x�N�g�������݂̍��W�ɑ���
	//m_cameraPos += velo;

	MyEngine::Vector3 shiftVec =  VTransform(kInitPos.CastVECTOR(),m_playerRotaMat);

	MyEngine::Vector3 pos = shiftVec;

	m_cameraPos = m_playerPos + pos;

	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	//�J�����̍��W��ݒ�

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
