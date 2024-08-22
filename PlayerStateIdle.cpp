#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateHitAttack.h"

#include "AttackBase.h"

#include "Player.h"

void PlayerStateIdle::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//�ړ����͂�����Ă�����
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0)
	{
		//State��Move�ɕύX����
		m_nextState = std::make_shared<PlayerStateMove>();
		return;
	}
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//State��NormalEnergyAttack�ɕύX����
			m_nextState =  std::make_shared<PlayerStateNormalEnergyAttack>();
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//State��NormalPhysicalAttack�ɕύX����
			m_nextState =  std::make_shared<PlayerStateNormalPhysicalAttack>();
			return;
		}
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//State��Dodge�ɕύX����
			m_nextState =  std::make_shared<PlayerStateDodge>();
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			m_nextState =  std::make_shared<PlayerStateGuard>();
			return;
		}
	}
	//�K�E�Z�p���b�g���J���Ă���ꍇ
	else
	{
		std::map<std::string, std::string> setSpecialAttack = player->GetSetSpecialAttack();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MP���\���ɂ�������
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kY]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (player->GetAttackKind(setSpecialAttack[Game::InputId::kY]))
				{
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
				}
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kB]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (player->GetAttackKind(setSpecialAttack[Game::InputId::kB]))
				{
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
				}
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MP���\���ɂ�������
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kX]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (player->GetAttackKind(setSpecialAttack[Game::InputId::kX]))
				{
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
				}
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MP���\���ɂ�������
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//�R�}���h�ɂ������K�E�Z���o��
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kA]);
				//����State��K�E�Z�̎�ނɉ����ĕύX����
				if (player->GetAttackKind(setSpecialAttack[Game::InputId::kA]))
				{
					m_nextState =  std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState =  std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
				}
			}
		}
	}


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
	m_nextState = std::make_shared<PlayerStateHitAttack>();
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	
	return damage;
}
