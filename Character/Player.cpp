#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "NormalAttack.h"
#include "SpecialAttack.h"
namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	constexpr float kAttackPos = 400;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_targetPos(),
	m_rota(0)
{
}

Player::~Player()
{
}

void Player::Init(Physics* physics)
{

	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_pPhysics = physics;
	Collidable::Init(physics);
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{

	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	//�C�����߂鏈��
	if (input.IsPress("Y"))
	{
		//�}�b�N�X����Ȃ����
		if (m_nowMp < m_status.mp)
		{
			m_nowMp++;
		}
	}

	//�U������
	Attack(scene, input);

	//�ړ�����
	velo = Move(velo, input);



	//���M�b�g�{�f�B�Ƀx���V�e�B��ݒ肷��
	m_rigidbody.SetVelo(velo);

	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	MV1DrawModel(m_modelHandle);
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//�X�e�B�b�N�ł̈ړ�
	{
		//�X�e�B�b�N�̓��͏��
		MyEngine::Input::StickInfo stick = input.GetStickInfo();
		//�v���C���[�̈ړ������x�N�g��
		MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

		//�v���C���[���㉺�ړ����邩���邩�ǂ���
		bool isMoveVertical = input.IsPress("LB");

		if (stickDir.sqLength() != 0)
		{

			stickDir = stickDir.Normalize();

			MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();

			//��]���x(���ړ��̑���)
			float HMoveSpeed = 0;

			if (stickDir.x != 0.0f)
			{
				HMoveSpeed = (stickDir.x * kMoveSpeed) / toTargetVec.Length();
			}

			DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", HMoveSpeed);

			m_rota += HMoveSpeed;

			//���E�ړ��͓G�̎��͂����
			velo.x += (m_targetPos.x + cosf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().x;
			velo.z += (m_targetPos.z + sinf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().z;

			/*if (isMoveVertical)
			{
				velo = MyEngine::Vector3();
				velo.y += -stickDir.z * kMoveSpeed;
			}
			else
			{
			}*/

			velo += toTargetVec.Normalize() * (stickDir.z * kMoveSpeed);
			DrawFormatString(400, 0, GetColor(255, 255, 255), "%f", velo.Length());;

			//MATRIX mat = MGetRotY(toTargetVec.y);

			//stickDir = stickDir.Normalize();


			//velo = stickDir * kHSpeed + toTargetVec * kHSpeed;

			//velo = velo.MatTransform(mat);

		}
	}

	//�L�[�{�[�h�ł̈ړ�

	//�X�e�B�b�N�ł̓��͂��s���Ă��Ȃ�������
	//if (velo.sqLength() == 0)
	//{
	//	//�v���C���[�̈ړ������x�N�g��
	//	MyEngine::Vector3 stickDir;
	//	//�����
	//	if (input.IsPress(Game::InputId::kUp))
	//	{
	//		stickDir.z = 1;
	//	}
	//	//������
	//	else if (input.IsPress(Game::InputId::kDown))
	//	{
	//		stickDir.z = -1;
	//	}
	//	//�E����
	//	if (input.IsPress(Game::InputId::kRight))
	//	{
	//		stickDir.x = 1;
	//	}
	//	//������
	//	else if (input.IsPress(Game::InputId::kLeft))
	//	{
	//		stickDir.x = -1;
	//	}
	//	//J����
	//	if (input.IsPress(Game::InputId::kLb))
	//	{
	//		stickDir.y = stickDir.z;
	//		stickDir.z = 0;
	//	}
	//	if (stickDir.sqLength() != 0)
	//	{
	//		MATRIX mat = MGetRotY(m_targetPos.y);

	//		stickDir = stickDir.Normalize();

	//		velo = stickDir * kHSpeed;

	//		velo = velo.MatTransform(mat);

	//	}
	//}

	return velo;
}

void Player::Attack(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//�v���C���[����G�ւ̃x�N�g�����쐬
	MyEngine::Vector3 toEnemyVec = m_targetPos - m_rigidbody.GetPos();

	//�U�����o�����W���쐬
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toEnemyVec.Normalize() * kAttackPos;

	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!input.IsPress("RB"))
	{
		//�C�e�U��
		if (input.IsTrigger("X"))
		{
			std::string attackId = "�C�e";
			//����MP�����݂�MP�������Ȃ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				//�C�e�U���̃|�C���^�쐬
				std::shared_ptr<NormalAttack> attack = std::make_shared<NormalAttack>(ObjectTag::kEnergyAttack);
				//�C�e�U���̏�����
				attack->Init(m_pPhysics, attackPos);
				//�X�e�[�^�X�ݒ�
				Game::AttackInfo status;
				status.damageRate = 10;
				status.speed = 80.0f;
				status.radius = 100.0f;
				status.isLaser = false;
				attack->SetStatus(m_attackData[attackId], m_targetPos);
				//�V�[���ɋC�e�U���ǉ�
				scene->AddAttack(attack);
			}
		}
		//�i���U��
		else if (input.IsTrigger("B"))
		{
			//�i���U���̃|�C���^�쐬
			std::shared_ptr<NormalAttack> attack = std::make_shared<NormalAttack>(ObjectTag::kPhysicalAttack);
			//�i���U���̏�����
			attack->Init(m_pPhysics, attackPos);
			//�X�e�[�^�X�ݒ�
			Game::AttackInfo status;
			status.damageRate = 30;
			status.speed = 0.0f;
			status.radius = 200.0f;
			status.isLaser = false;
			attack->SetStatus(status, m_targetPos);
			//�V�[���Ɋi���U���ǉ�
			scene->AddAttack(attack);
		}
	}
	//�K�E�Z�p���b�g���J���Ă���Ƃ�
	else
	{
		//�O���t�@�C���ǂݍ��݂����遫

		//���߂͂ߔg
		if (input.IsTrigger("Y"))
		{
			//�K�E�Z�̃|�C���^�쐬
			std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>(ObjectTag::kEnergyAttack);
			//�K�E�Z�̏�����
			attack->Init(m_pPhysics, attackPos);
			//�X�e�[�^�X�ݒ�
			Game::AttackInfo status;
			status.damageRate = 10;
			status.speed = 80.0f;
			status.radius = 100.0f;
			status.isLaser = true;
			attack->SetStatus(status, m_targetPos);
			//�V�[���ɕK�E�Z�U�ǉ�
			scene->AddAttack(attack);
		}
		//�i���K�E1
		else if (input.IsTrigger("B"))
		{
			//�K�E�Z�̃|�C���^�쐬
			std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>(ObjectTag::kPhysicalAttack);
			//�K�E�Z�̏�����
			attack->Init(m_pPhysics, attackPos);
			//�X�e�[�^�X�ݒ�
			Game::AttackInfo status;
			status.damageRate = 30;
			status.speed = 0.0f;
			status.radius = 200.0f;
			status.isLaser = false;
			attack->SetStatus(status, m_targetPos);
			//�V�[���ɕK�E�Z�ǉ�
			scene->AddAttack(attack);
		}
		//�i���K�E2
		else if (input.IsTrigger("X"))
		{
			//�K�E�Z�̃|�C���^�쐬
			std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>(ObjectTag::kPhysicalAttack);
			//�K�E�Z�̏�����
			attack->Init(m_pPhysics, attackPos);
			//�X�e�[�^�X�ݒ�
			Game::AttackInfo status;
			status.damageRate = 30;
			status.speed = 0.0f;
			status.radius = 200.0f;
			status.isLaser = false;
			attack->SetStatus(status, m_targetPos);
			//�V�[���ɕK�E�Z�ǉ�
			scene->AddAttack(attack);
		}
		//�C�e�A��
		else if (input.IsTrigger("A"))
		{
			for (int i = 0; i < 30; i++)
			{
				//�K�E�Z�̃|�C���^�쐬
				std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>(ObjectTag::kEnergyAttack);
				//�K�E�Z�̏�����
				MyEngine::Vector3 pos = attackPos;
				pos.x += GetRand(800) - 400;
				pos.y += GetRand(800) - 400;
				pos.z += GetRand(800) - 400;
				attack->Init(m_pPhysics, pos);
				//�X�e�[�^�X�ݒ�
				Game::AttackInfo status;
				status.damageRate = 30;
				status.speed = 100.0f;
				status.radius = 200.0f;
				status.isLaser = false;
				attack->SetStatus(status, m_targetPos);
				//�V�[���ɕK�E�Z�ǉ�
				scene->AddAttack(attack);
			}
		}
	}

}
