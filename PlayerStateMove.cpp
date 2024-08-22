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
void PlayerStateMove::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//Stateの変化
	//必殺技パレットを開いていない場合
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//StateをNormalEnergyAttackに変更する
			m_nextState = std::make_shared<PlayerStateNormalEnergyAttack>();
			return;
		}
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			//StateをNormalPhysicalAttackに変更する
			m_nextState = std::make_shared<PlayerStateNormalPhysicalAttack>();
			return;
		}
		//ガード入力がされていたら
		if (input.IsPress(Game::InputId::kRb))
		{
			//StateをGuardに変更する
			m_nextState = std::make_shared<PlayerStateGuard>();
			return;
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
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
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
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
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
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
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
					m_nextState = std::make_shared<PlayerStateSpecialEnergyAttack>();
					return;
				}
				else
				{
					m_nextState = std::make_shared<PlayerStateSpecialPhysicalAttack>();
					return;
				}
			}
		}
	}

	//上記の状態遷移に当てはまらなかったら
	//下の移動処理を行う

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, stick.leftStickY);

	//移動入力がされているとき
	if (dir.sqLength() != 0)
	{
		//回避行動の入力がされたら
		if (input.IsTrigger(Game::InputId::kA))
		{
			//StateをDodgeに変更する
			m_nextState = std::make_shared<PlayerStateDodge>();
			//回避の方向を設定する
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			PlayerStateDodge::MoveDir dodgeDir = PlayerStateDodge::MoveDir::kFront;
			//入力が一番大きい方向に回避を行う


			state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
			return;
		}
		m_nextState = shared_from_this();
		return;
	}
	//移動入力がされていないときに
	else
	{
		//回避行動の入力がされたら
		if (input.IsTrigger(Game::InputId::kA))
		{
			//StateをDodgeに変更する
			m_nextState = std::make_shared<PlayerStateDodge>();
			//回避の方向を設定する
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
			return;
		}
		//何も入力がされていなければ
		m_nextState = std::make_shared<PlayerStateIdle>();
	}

}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}
