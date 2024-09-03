#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
#include "CapsuleColliderData.h"
#include "PlayerStateAll.h"
namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	//�U�����o�����W
	constexpr float kAttackPos = 500.0f;
	//�i���U�����o�����̓G�Ƃ̋���(�����蔻��̑傫���̔{��)
	constexpr float kPhysicalAttackLange = 0.9f;
	//�����蔻��̑傫��
	constexpr float kColScale = 100.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0),
	m_lastAttackTime(0),
	m_isOpenSpecialPallet(false)
{
	LoadAnimationData(true);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	std::shared_ptr<Collidable> p = shared_from_this();
	auto p2 = std::dynamic_pointer_cast<Player>(p);
	m_pState = std::make_shared<PlayerStateIdle>(p2);
	m_pState->m_nextState = m_pState;

	SetSpecialAttack();

	MV1SetScale(m_modelHandle, VGet(3, 3, 3));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = 10;

	m_pPhysics = physics;
	Collidable::Init(physics);

	//Y���𒆐S�Ƃ�����]������̂�
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	m_rigidbody.SetPos(pos);

	//�A�j���[�V����
	ChangeAnim("Idle");

	//�����蔻��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;
	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = 10;


	//Y���𒆐S�Ƃ�����]������̂�
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();


	//�A�j���[�V�����̏�����
	ChangeAnim("Idle");

	//�����蔻��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);

	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;

	m_rigidbody.SetPos(pos);

	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
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
		Attack(input);

		//�Ō�ɍU���������Ԃ��v������
		m_lastAttackTime++;

//		MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	}
	//����ł��Ȃ�����
	else
	{
		//�����Ȃ����Ԃ����炷
		m_stanTime--;
		//�U�����o���Ă����Ԃ�������
		if (m_isAttack)
		{
			//1����ōU���𕡐���o���Z�ł����
			if (m_attackData[m_attackId].attackNum > 1)
			{
				//���U�����o���n�߂ĉ��b��������
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
					m_lastAttackTime = 0;
				}
			}
			//�P���U���ł����
			else
			{
				//�i���U���������ꍇ
				if (m_attackData[m_attackId].isEnergy == false)
				{
					//�U���̓����蔻��̋���
					float attackLange = ((m_targetPos - m_rigidbody.GetPos()).Normalize() * kAttackPos).Length() + m_attackData[m_attackId].radius * kPhysicalAttackLange;

					//�G�Ƃ̋������߂��Ȃ�����
					if ((m_targetPos - m_rigidbody.GetPos()).Length() < attackLange)
					{
						m_isNearTarget = true;
					}

					//�܂��G�Ƃ̋������������
					if (!m_isNearTarget)
					{
						//�G�ɋ߂Â�
						velo = (m_targetPos - m_rigidbody.GetPos()).Normalize() * kMoveSpeed;
						//�G���߂��ɂ��Ȃ��ꍇ�U���̃J�E���g���~�߂�
						m_stanTime++;
					}

					//�U�����[�V�����ɓ����ĉ��b�����߂�
					int time = m_attackData[m_attackId].actionTime - m_stanTime;
					//�U�����o�����ԂɂȂ�����
					if (time > m_attackData[m_attackId].attackTime)
					{
						//�U�����o��
						scene->AddAttack(CreateAttack(m_pPhysics, m_attackId, true));
						m_isAttack = false;
						m_lastAttackTime = 0;
					}
				}
				//�C�e�U���������ꍇ
				else
				{
					//�U�����[�V�����ɓ����ĉ��b�����߂�
					int time = m_attackData[m_attackId].actionTime - m_stanTime;
					//�U�����o�����ԂɂȂ�����
					if (time > m_attackData[m_attackId].attackTime)
					{
						//�U�����o��
						scene->AddAttack(CreateAttack(m_pPhysics, m_attackId, true));
						m_isAttack = false;
						m_lastAttackTime = 0;
					}

				}
			}
		}
	}
	//�K�E�Z�p���b�g���J���Ă��邩�ǂ�����ۑ�����
	m_isOpenSpecialPallet = input.IsPress(Game::InputId::kLb);


	//�O�̃t���[����State���r���ĈႤState��������
	if (m_pState->m_nextState->GetKind() != m_pState->GetKind())
	{
		//State��ύX����
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	//State�̍X�V
	m_pState->Update(input);

	//�A�j���[�V������i�߂�
	PlayAnim();

	//�����蔻��̍X�V
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;
	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//�̗͂�0�ȉ��ɂȂ�ƃQ�[���I�[�o�[
	if (m_nowHp <= 0)
	{
		scene->GameOver();
	}
#ifdef _DEBUG
	if (input.IsPress(Game::InputId::kSelect))
	{
		m_nowHp -= 1000;
	}
#endif
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "HP:%f,MP:%f", m_nowHp, m_nowMp);
	MV1DrawModel(m_modelHandle);
}
void Player::Attack(std::string id)
{
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);

	//�U�����o�����W���쐬
	MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	ans->Init(m_pPhysics, attackPos);
	//�X�e�[�^�X�ݒ�
	ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);

	m_pScene->AddAttack(ans);
}
bool Player::GetAttackKind(std::string attackId)
{
	return m_attackData[attackId].isEnergy;
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - static_cast<int>(m_status.def);
		if (damage < 0)
		{
			damage = 2;
		}

		m_nowHp -= damage;
	}
}

std::map<std::string, std::string> Player::GetSetSpecialAttackName()
{
	std::map<std::string, std::string> ans;

	ans[Game::InputId::kA] = m_attackData[m_setSpecialAttack[Game::InputId::kA]].name;
	ans[Game::InputId::kB] = m_attackData[m_setSpecialAttack[Game::InputId::kB]].name;
	ans[Game::InputId::kX] = m_attackData[m_setSpecialAttack[Game::InputId::kX]].name;
	ans[Game::InputId::kY] = m_attackData[m_setSpecialAttack[Game::InputId::kY]].name;

	return ans;
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//�X�e�B�b�N�ł̈ړ�

	//�X�e�B�b�N�̓��͏��
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//�v���C���[�̈ړ������x�N�g��
	MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

	//�v���C���[���㉺�ړ����邩���邩�ǂ���
	bool isMoveVertical = input.IsPress(Game::InputId::kLb);

	if (stickDir.sqLength() != 0)
	{

		stickDir = stickDir.Normalize();

		//Y���𒆐S�Ƃ�����]������̂�
		MyEngine::Vector3 rotationShaftPos = m_targetPos;
		//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
		rotationShaftPos.y = m_rigidbody.GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

		//��]���x(���ړ��̑���)
		float hMoveSpeed = 0;

		if (stickDir.x != 0.0f)
		{
			hMoveSpeed = (stickDir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		//MyEngine::Vector3 a = rotationShaftPos - m_rigidbody.GetPos();

		//m_rota = atan2f(a.z,a.x);

		m_rota += hMoveSpeed;

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

void Player::Attack(MyEngine::Input input)
{

	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!input.IsPress("LB"))
	{
		//�C�e�U��
		if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kEnergyAttack1;
			//����MP�����݂�MP�������Ȃ�������
			if (m_nowMp >= GetAttackCost(attackId))
			{
				SetNormalAttack(false, m_lastAttackTime);
			}
		}
		//�i���U��
		else if (input.IsTrigger("B"))
		{
			SetNormalAttack(true, m_lastAttackTime);
		}
	}
	//�K�E�Z�p���b�g���J���Ă���Ƃ�
	else
	{
		//���߂͂ߔg
		if (input.IsTrigger("Y"))
		{
			//MP���\���ɂ�������
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["Y"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["Y"]);
			}
		}
		//�i���K�E1
		else if (input.IsTrigger("B"))
		{
			//MP���\���ɂ�������
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["B"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["B"]);
			}
		}
		//�i���K�E2
		else if (input.IsTrigger("X"))
		{
			//MP���\���ɂ�������
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["X"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["X"]);
			}
		}
		//�C�e�A��
		else if (input.IsTrigger("A"))
		{
			//MP���\���ɂ�������
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["A"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["A"]);
			}
		}
	}

}

void Player::ChangeState(std::shared_ptr<PlayerStateBase> state)
{
	m_pState = state;
}

void Player::SetSpecialAttack()
{
	m_setSpecialAttack["A"] = CommandId::kSpEnergyAttack;
	m_setSpecialAttack["B"] = CommandId::kSpStanAttack;
	m_setSpecialAttack["X"] = CommandId::kSpSlamAttack;
	m_setSpecialAttack["Y"] = CommandId::kSpLaserAttack;

}
