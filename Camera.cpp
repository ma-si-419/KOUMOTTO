#include "Camera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 1500.0f;
	//�J�����̈ړ����x(�����̔{��)
	constexpr float kCameraSpeed = 0.1f;
	//�J�����̍���(�v���C���[���猩��)
	constexpr float kCameraHeight = 1500.0f;
	//��]�����Ȃ��P�\�͈�
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
	//�v���C���[�̉�]�Ɗ��S�Ɉ�v���Ȃ��悤�ɃJ�����������Ȃ��P�\����������
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

	//�ړ���
	MyEngine::Vector3 velo;

	//�G�l�~�[���猩�ăv���C���[�̊p�x���Q�Ƃ����W�����肷��
	//��]�̒��S�Ƃ�����W
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	MyEngine::Vector3 centerToPlayer = (m_playerPos - rotationShaftPos);
	//��]�̒��S����v���C���[�Ɍ������x�N�g�����쐬��
	//�v���C���[�̌��ɃJ�����������Ă���

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
