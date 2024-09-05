#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateAttack.h"
#include "Player.h"

namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 5.0f;
	//�g���K�[����������
	constexpr int kTriggerReaction = 200;
}

void PlayerStateMove::Init()
{
	m_pPlayer->ChangeAnim("Move");
}

void PlayerStateMove::Update(MyEngine::Input input)
{
	//State�̕ω�
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX, false);
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
			state->Init();
			return;
		}
	}
	//�K�E�Z�p���b�g���J���Ă���ꍇ
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttackId();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kB, true);
				return;
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kX, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
			}
		}
	}

	//��L�̏�ԑJ�ڂɓ��Ă͂܂�Ȃ�������
	//���̈ړ��������s��

	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//�ړ����͂�����Ă���Ƃ�
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();

		//Y���𒆐S�Ƃ�����]������̂�
		MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
		//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
		rotationShaftPos.y = m_pPlayer->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

		//��]���x(���ړ��̑���)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		MyEngine::Vector3 a = rotationShaftPos - m_pPlayer->GetPos();

		//m_rota = atan2f(a.z,a.x);

		m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

		//���E�ړ��͓G�̎��͂����

		//�G�̍��W����]�x���Q�Ƃ��A���̉�]�x�������玟�͂ǂ̍��W�ɂȂ邩�v�Z��
		//���݂̍��W���炻�̍��W�Ɍ������x�N�g�����쐬����
		velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

		//�㉺�ړ����͂���Ă�����
		if (input.GetTriggerInfo().left > kTriggerReaction)
		{
			//�O����͂��㉺�̃x�N�g���ɕϊ�
			velo.y += dir.z * kMoveSpeed;
		}
		//����Ă��Ȃ������ꍇ
		else
		{
			MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
			//�G�ɋ߂���������͂����悤�ɂ���
			if (toCenterVec.Length() > 2000)
			{
				//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
				toCenterVec.y = 0;
				velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
			}
			else
			{
				//�O���͂����ړ��Ɍ����͂���]�̒��S���痣���x�N�g���ɕϊ�
				toCenterVec.y = 0;
				if (dir.z > 0)
				{
					hMoveSpeed = (dir.z * kMoveSpeed) / toShaftPosVec.Length();
					m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);
					velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
					velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;
				}
				else if(dir.z < 0)
				{
					velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
				}
			}
		}

		//�K�E�Z�p���b�g���J����Ă��Ȃ�
		if (!input.IsPress(Game::InputId::kLb))
		{
			//����s���̓��͂����ꂽ��
			if (input.IsTrigger(Game::InputId::kA))
			{
				//State��Dodge�ɕύX����
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
				//����̕�����ݒ肷��
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				state->Init(dir);
				return;
			}
		}
		m_pPlayer->PlayAnim();
		m_pPlayer->SetVelo(velo);

		//���ʂ̍��W
		MyEngine::Vector3 frontPos = m_pPlayer->GetPos() + velo;
		//�㉺�ړ����s���ۂ͓G�̕��������Ȃ��瓮���悤�ɂ���
		if (frontPos.y != m_pPlayer->GetPos().y)
		{
			frontPos = m_pPlayer->GetTargetPos();
		}
		//�ړ���̍��W�������悤�ɂ���
		m_pPlayer->SetModelFront(frontPos);
		//�����̃|�C���^��Ԃ�
		m_nextState = shared_from_this();
		return;
	}
	//�ړ����͂�����Ă��Ȃ��Ƃ���
	else
	{
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//State��Dodge�ɕύX����
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			//�G�Ɍ������x�N�g�����쐬����
			MyEngine::Vector3 dir = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Normalize();
			state->Init(dir);
			return;
		}
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer,m_pScene);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);

	return damage;
}