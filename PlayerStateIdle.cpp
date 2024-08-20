#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateSpecialPhysicalAttack.h"

#include "Player.h"

std::shared_ptr<PlayerStateBase> PlayerStateIdle::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//�ړ����͂�����Ă�����
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0)
	{
		//State��Move�ɕύX����
		return std::make_shared<PlayerStateMove>();
	}
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//State��NormalEnergyAttack�ɕύX����
			return std::make_shared<PlayerStateNormalEnergyAttack>();
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//State��NormalPhysicalAttack�ɕύX����
			return std::make_shared<PlayerStateNormalPhysicalAttack>();
		}
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//State��Dodge�ɕύX����
			return std::make_shared<PlayerStateDodge>();
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			return std::make_shared<PlayerStateGuard>();
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
					return std::make_shared<PlayerStateSpecialEnergyAttack>();
				}
				else
				{
					return std::make_shared<PlayerStateSpecialPhysicalAttack>();
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
					return std::make_shared<PlayerStateSpecialEnergyAttack>();
				}
				else
				{
					return std::make_shared<PlayerStateSpecialPhysicalAttack>();
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
					return std::make_shared<PlayerStateSpecialEnergyAttack>();
				}
				else
				{
					return std::make_shared<PlayerStateSpecialPhysicalAttack>();
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
					return std::make_shared<PlayerStateSpecialEnergyAttack>();
				}
				else
				{
					return std::make_shared<PlayerStateSpecialPhysicalAttack>();
				}
			}
		}
	}


	//��L�̓��͂�����Ă��Ȃ�������State��ύX���Ȃ�
	return shared_from_this();
}
