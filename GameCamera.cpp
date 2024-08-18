#include "GameCamera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 2000.0f;
	//�J�����̈ړ����x(�����̔{��)
	constexpr float kCameraSpeed = 0.1f;
	//�J�����̍���(�v���C���[���猩��)
	constexpr float kCameraHeight = 1000.0f;
	//��]�����Ȃ��P�\�͈�
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
	//�ړ���
	MyEngine::Vector3 velo;

	//�G�l�~�[���猩�ăv���C���[�̊p�x���Q�Ƃ����W�����肷��
	//��]�̒��S�Ƃ�����W
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//��]�̒��S����v���C���[�Ɍ������x�N�g�����쐬��
	//�v���C���[�̌��ɃJ�����������Ă���
	MyEngine::Vector3 centerToPlayer = (m_playerPos - rotationShaftPos);

	rotationShaftPos.y = m_playerPos.y + kCameraHeight;
	//���Ɍ��������W
	MyEngine::Vector3 pos;
	//��]�̒��S���獡�̍��W���v�Z����
	pos.x = rotationShaftPos.x + cosf(m_rota) * centerToPlayer.Length();
	pos.y = rotationShaftPos.y;
	pos.z = rotationShaftPos.z + sinf(m_rota) * centerToPlayer.Length();

	//���S����v���C���[�̋����ɂ���ăJ�����̋�����ς���
	pos += centerToPlayer.Normalize() * kPlayerDistance;
	//���̍��W�Ɍ������ړ��x�N�g�����쐬
	velo = pos - m_cameraPos;
	//�ړ��x�N�g�������݂̍��W�ɑ���
	m_cameraPos += velo;
	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;
	
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

	float margin = kRotaMargin / centerToPlayer.Length();
	//�v���C���[�̐^���������ƃJ���������Ă����Ȃ��悤��
	//�����ɗP�\����������
	if (m_rota > m_playerRota + margin)
	{
		m_rota -= m_rota - m_playerRota - margin;
	}
	else if (m_rota < m_playerRota - margin)
	{
		m_rota += m_playerRota - m_rota - margin;
	}
}
