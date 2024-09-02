#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateHitAttack.h"


void PlayerStateIdle::Update(MyEngine::Input input)
{
	//�ړ����͂�����Ă�����
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0)
	{
		//State��Move�ɕύX����
		m_nextState = std::make_shared<PlayerStateMove>(m_pPlayer);
		//�A�j���[�V������ω�������
		m_pPlayer->ChangeAnim("MoveFront");
		return;
	}
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//State��NormalEnergyAttack�ɕύX����
			m_nextState =  std::make_shared<PlayerStateNormalEnergyAttack>(m_pPlayer);
			//�A�j���[�V�������C�e�U���ɕω�������
			//m_pPlayer->ChangeAnim();
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//State��NormalPhysicalAttack�ɕύX����
			m_nextState =  std::make_shared<PlayerStateNormalPhysicalAttack>(m_pPlayer);
			//�A�j���[�V������G�Ɍ������Ă����A�j���[�V�����ɕω�������
			m_pPlayer->ChangeAnim("MoveFront");
			return;
		}
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//State��Dodge�ɕύX����
			m_nextState =  std::make_shared<PlayerStateDodge>(m_pPlayer);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
			state->SetMoveDir(MyEngine::Vector3(0,0,1));
			//�ړ������ɍ��킹�ăA�j���[�V������ω�������
			//m_pPlayer->ChangeAnim();
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			m_nextState =  std::make_shared<PlayerStateGuard>(m_pPlayer);
			//�A�j���[�V�������K�[�h�ɕω�������
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
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
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
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
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
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
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
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>(m_pPlayer);
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>(m_pPlayer);
					return;
				}
			}
		}
	}

	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//��L�̓��͂�����Ă��Ȃ�������State��ύX���Ȃ�
	m_nextState =  shared_from_this();
}

int PlayerStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
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
