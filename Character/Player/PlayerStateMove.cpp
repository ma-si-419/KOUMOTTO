#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"

namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	//�_���[�W�̌y����
	constexpr float kDamageCutRate = 0.25f;

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
			//State��NormalEnergyAttack�ɕύX����
			m_nextState = std::make_shared<PlayerStateNormalEnergyAttack>(m_pPlayer);
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//State��NormalPhysicalAttack�ɕύX����
			m_nextState = std::make_shared<PlayerStateNormalPhysicalAttack>(m_pPlayer);
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer);
			m_pPlayer->ChangeAnim("Guard");
			return;
		}
	}
	//�K�E�Z�p���b�g���J���Ă���ꍇ
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttack();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kY]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (m_pPlayer->GetAttackKind(setSpecialAttack[Game::InputId::kY]))
				{
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
					return;
				}
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kB]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (m_pPlayer->GetAttackKind(setSpecialAttack[Game::InputId::kB]))
				{
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
					return;
				}
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kX]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (m_pPlayer->GetAttackKind(setSpecialAttack[Game::InputId::kX]))
				{
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
					return;
				}
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kA]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (m_pPlayer->GetAttackKind(setSpecialAttack[Game::InputId::kA]))
				{
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
					return;
				}
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
		if (input.IsPress(Game::InputId::kLb))
		{
			//�O����͂��㉺�̃x�N�g���ɕϊ�
			velo.y += dir.z * kMoveSpeed;
		}
		//����Ă��Ȃ������ꍇ
		else
		{
			//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
			MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
		}

		//�K�E�Z�p���b�g���J����Ă��Ȃ�
		if (!input.IsPress(Game::InputId::kLb))
		{
			//����s���̓��͂����ꂽ��
			if (input.IsTrigger(Game::InputId::kA))
			{
				//State��Dodge�ɕύX����
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer);
				//����̕�����ݒ肷��
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				PlayerStateDodge::MoveDir dodgeDir = PlayerStateDodge::MoveDir::kFront;
				//���͂���ԑ傫�������ɉ�����s��


				state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
				state->SetMoveDir(dir);
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
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
			state->SetMoveDir(dir.Normalize());
			return;
		}
		//�������͂�����Ă��Ȃ����
		m_pPlayer->ChangeAnim("Idle");
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
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
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}
