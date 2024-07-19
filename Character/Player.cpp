#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	//�U�����o�����W
	constexpr float kAttackPos = 500.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0)
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	MV1SetScale(m_modelHandle, VGet(300, 300, 300));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_pPhysics = physics;
	Collidable::Init(physics);
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	m_stanTime--;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;
	//�X�^�����͑���ł��Ȃ��悤�ɂ���
	if (m_stanTime < 0)
	{

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
		//�X�^�����Ă��Ȃ����͏�ɓG�̕���������������
		//�v���C���[����ɃG�l�~�[�̕����Ɍ�����
		//TODO�F�^�[�Q�b�g�����ꂽ���Ɉ�u�Ō����悤�ɂȂ��Ă邩�炻���
		//��u�ł͂Ȃ������Ă��銴�����o��
		MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	}
	//����ł��Ȃ�����
	else
	{
		//�U�����o���Ă����Ԃ�������
		if (m_isAttack)
		{
			//�U���𕡐���o���Z�ł����
			if (m_attackData[m_attackId].attackNum > 1)
			{
				//���U�����o���n�߂ĉ��b��
				int time = m_attackData[m_attackId].actionTime - m_stanTime;
				//�U���̃X�p�����擾����
				int span = m_attackData[m_attackId].attackTime / m_attackData[m_attackId].attackNum;
				//�U�����o���؂鎞�Ԃ܂ł����U�����o���Ȃ��悤�ɂ���
				if (time % span == 0 && time <= m_attackData[m_attackId].attackTime)
				{
					//�U�����쐬
					std::shared_ptr<AttackBase> attack = CreateAttack(m_pPhysics, m_attackId, true);
					//���[�U�[��̍U���ł���Ώ����鎞�Ԃ����낦��
					if (m_attackData[m_attackId].isLaser)
					{
						//������܂ł̎���
						int lifeTime = m_attackData[m_attackId].lifeTime - time;

						attack->SetAttackTime(lifeTime);
					}
					//�U�����o��
					scene->AddAttack(attack);
				}
			}
			//�U������x�݂̂ł����
			else
			{
				//�U�����o��
				scene->AddAttack(CreateAttack(m_pPhysics, m_attackId,true));
				m_isAttack = false;
			}
		}
	}
	//���M�b�g�{�f�B�Ƀx���V�e�B��ݒ肷��
	m_rigidbody.SetVelo(velo);

	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "HP:%d,MP:%d", m_nowHp, m_nowMp);
	MV1DrawModel(m_modelHandle);
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - m_status.def;
		if (damage < 0)
		{
			damage = 2;
		}

		m_nowHp -= damage;
	}
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

			//Y���𒆐S�Ƃ�����]������̂�
			MyEngine::Vector3 rotationShaftPos = m_targetPos;
			//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
			rotationShaftPos.y = m_rigidbody.GetPos().y;

			MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

			//��]���x(���ړ��̑���)
			float HMoveSpeed = 0;

			if (stickDir.x != 0.0f)
			{
				HMoveSpeed = (stickDir.x * kMoveSpeed) / toShaftPosVec.Length();
			}

			DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", HMoveSpeed);

			/*MyEngine::Vector3 a = rotationShaftPos - m_rigidbody.GetPos();

			m_rota = atan2f(a.z,a.x);*/

			m_rota += HMoveSpeed;

			//���E�ړ��͓G�̎��͂����

			//�G�̍��W����]�x���Q�Ƃ��A���̉�]�x�������玟�͂ǂ̍��W�ɂȂ邩�v�Z��
			//���݂̍��W���炻�̍��W�Ɍ������x�N�g�����쐬����
			velo.x = (rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length()) - m_rigidbody.GetPos().x;
			velo.z = (rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length()) - m_rigidbody.GetPos().z;

			//�㉺�ړ����͂���Ă�����
			if (isMoveVertical)
			{
				//�O����͂��㉺�̃x�N�g���ɕϊ�
				velo.y += stickDir.z * kMoveSpeed;
			}
			//����Ă��Ȃ������ꍇ
			else
			{
				//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
				MyEngine::Vector3 toCenterVec = m_targetPos - m_rigidbody.GetPos();
				toCenterVec.y = 0;
				velo += toCenterVec.Normalize() * (stickDir.z * kMoveSpeed);
			}


			DrawFormatString(400, 0, GetColor(255, 255, 255), "%f", velo.Length());;

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

	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!input.IsPress("RB"))
	{
		//�C�e�U��
		if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kEnergyAttack;
			//����MP�����݂�MP�������Ȃ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				//�C�e�U���݈̂ړ����ɏo����Z�ł���̂ł����ŋZ���o��
				m_nowMp -= m_attackData[attackId].cost;
				m_attackTarget = m_targetPos;
				scene->AddAttack(CreateAttack(m_pPhysics, attackId,true));

			}
		}
		//�i���U��
		else if (input.IsTrigger("B"))
		{
			std::string attackId = CommandId::kPhysicalAttack;
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
	}
	//�K�E�Z�p���b�g���J���Ă���Ƃ�
	else
	{
		//���߂͂ߔg
		if (input.IsTrigger("Y"))
		{
			std::string attackId = CommandId::kSpLaserAttack;
			//MP���\���ɂ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//�i���K�E1
		else if (input.IsTrigger("B"))
		{
			std::string attackId = CommandId::kSpStanAttack;
			//MP���\���ɂ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//�i���K�E2
		else if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kSpSlamAttack;
			//MP���\���ɂ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//�C�e�A��
		else if (input.IsTrigger("A"))
		{
			std::string attackId = CommandId::kSpEnergyAttack;
			//MP���\���ɂ�������
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
	}

}
