#include "Camera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 1000.5f;
	//�f�t�H���g�̃J�����̈ʒu
	const MyEngine::Vector3 kDefaultPos(500, 500, -5000);
	//�J�����̈ړ����x(�����̔{��)
	constexpr float kCameraSpeed = 0.1f;
}

Camera::Camera()
{
	SetCameraNearFar(10.0f, 30000.0f);
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_cameraPos = m_playerPos + kDefaultPos;
}

void Camera::Update()
{
	//�G�l�~�[����v���C���[�ւ̃x�N�g���쐬
	MyEngine::Vector3 targetToPlayerVec = m_playerPos - m_targetPos;
	targetToPlayerVec = targetToPlayerVec.Normalize();

	//X����]������
//	MATRIX x = MGetRotX(-10);
	//Y����]������
	MATRIX m = MGetRotY(10);

//	MATRIX m = MMult(x, y);
	MyEngine::Vector3 moveVec = VTransformSR(targetToPlayerVec.CastVECTOR(),m);

	m_cameraPos = m_playerPos - moveVec * kPlayerDistance;


	MyEngine::Vector3 cameraTarget = m_playerPos - (m_playerPos - m_targetPos) / 2;

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
}
