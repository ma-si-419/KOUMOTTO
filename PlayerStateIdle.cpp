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
	//移動入力がされていたら
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0)
	{
		//StateをMoveに変更する
		return std::make_shared<PlayerStateMove>();
	}
	//必殺技パレットを開いていない場合
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//StateをNormalEnergyAttackに変更する
			return std::make_shared<PlayerStateNormalEnergyAttack>();
		}
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			//StateをNormalPhysicalAttackに変更する
			return std::make_shared<PlayerStateNormalPhysicalAttack>();
		}
		//回避行動の入力がされたら
		if (input.IsTrigger(Game::InputId::kA))
		{
			//StateをDodgeに変更する
			return std::make_shared<PlayerStateDodge>();
		}
		//ガード入力がされていたら
		if (input.IsPress(Game::InputId::kRb))
		{
			//StateをGuardに変更する
			return std::make_shared<PlayerStateGuard>();
		}
	}
	//必殺技パレットを開いている場合
	else
	{
		std::map<std::string, std::string> setSpecialAttack = player->GetSetSpecialAttack();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MPが十分にあったら
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//コマンドにあった必殺技を出す
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kY]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//コマンドにあった必殺技を出す
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kB]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//コマンドにあった必殺技を出す
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kX]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (player->GetNowMp() >= player->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//コマンドにあった必殺技を出す
				player->PlaySpecialAttack(setSpecialAttack[Game::InputId::kA]);
				//次のStateを必殺技の種類に応じて変更する
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


	//上記の入力がされていなかったらStateを変更しない
	return shared_from_this();
}
