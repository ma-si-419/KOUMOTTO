#include "Player.h"
#include "Game.h"
namespace
{
	constexpr float kSpeed = 18.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
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
	
	//�ړ������̌v�Z
	velo = Move(velo,input);

	//���M�b�g�{�f�B�Ƀx���V�e�B��ݒ肷��
	m_rigidbody.SetVelo(velo);

	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo,MyEngine::Input input)
{
	//�X�e�B�b�N�ł̈ړ�
	{
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
	}

	//�L�[�{�[�h�ł̈ړ�

	//�X�e�B�b�N�ł̓��͂��s���Ă��Ȃ�������
	if (velo.sqLength() == 0)
	{
		//�v���C���[�̈ړ������x�N�g��
		MyEngine::Vector3 dir;
		//�����
		if (input.IsPress(Game::InputId::kUp))
		{
			dir.z = 1;
		}
		//������
		else if (input.IsPress(Game::InputId::kDown))
		{
			dir.z = -1;
		}
		//�E����
		if (input.IsPress(Game::InputId::kRight))
		{
			dir.x = 1;
		}
		//������
		else if (input.IsPress(Game::InputId::kLeft))
		{
			dir.x = -1;
		}
		//J����
		if (input.IsPress(Game::InputId::kLb))
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
	}

	return velo;
}
