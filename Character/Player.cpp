#include "Player.h"

namespace
{
	constexpr float kSpeed = 18.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1",ObjectTag::kPlayer),
	m_cameraDir()
{
}

Player::~Player()
{
}

void Player::Init(Physics* physics)
{
	Collidable::Init(physics);
}

void Player::Update(MyEngine::Input input)
{
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;
	//�X�e�B�b�N�̓��͏��
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//�v���C���[�̈ړ������x�N�g��
	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);
	if (input.IsPress("LB"))
	{
		dir.y = dir.z;
		dir.z = 0;
	}
	if (dir.sqLength() != 0)
	{
		MATRIX mat = MGetRotY(m_cameraDir.y);

		dir = dir.Normalize();

		velo = dir * kSpeed;

		velo = velo.MatTransform(mat);

	}
	m_rigidbody.SetVelo(velo);

	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}
