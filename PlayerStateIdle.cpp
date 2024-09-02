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
	//移動入力がされていたら
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0)
	{
		//StateをMoveに変更する
		m_nextState = std::make_shared<PlayerStateMove>(m_pPlayer);
		//アニメーションを変化させる
		m_pPlayer->ChangeAnim("MoveFront");
		return;
	}
	//必殺技パレットを開いていない場合
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//StateをNormalEnergyAttackに変更する
			m_nextState =  std::make_shared<PlayerStateNormalEnergyAttack>(m_pPlayer);
			//アニメーションを気弾攻撃に変化させる
			//m_pPlayer->ChangeAnim();
			return;
		}
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			//StateをNormalPhysicalAttackに変更する
			m_nextState =  std::make_shared<PlayerStateNormalPhysicalAttack>(m_pPlayer);
			//アニメーションを敵に向かっていくアニメーションに変化させる
			m_pPlayer->ChangeAnim("MoveFront");
			return;
		}
		//回避行動の入力がされたら
		if (input.IsTrigger(Game::InputId::kA))
		{
			//StateをDodgeに変更する
			m_nextState =  std::make_shared<PlayerStateDodge>(m_pPlayer);
			//回避の方向を設定する
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->SetAnimDir(PlayerStateDodge::MoveDir::kFront);
			state->SetMoveDir(MyEngine::Vector3(0,0,1));
			//移動方向に合わせてアニメーションを変化させる
			//m_pPlayer->ChangeAnim();
			return;
		}
		//ガード入力がされていたら
		if (input.IsPress(Game::InputId::kRb))
		{
			//StateをGuardに変更する
			m_nextState =  std::make_shared<PlayerStateGuard>(m_pPlayer);
			//アニメーションをガードに変化させる
			m_pPlayer->ChangeAnim("Guard");
			return;
		}
	}
	//必殺技パレットを開いている場合
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttack();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//コマンドにあった必殺技を出す
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kY]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//コマンドにあった必殺技を出す
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kB]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//コマンドにあった必殺技を出す
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kX]);
				//次のStateを必殺技の種類に応じて変更する
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
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//コマンドにあった必殺技を出す
				m_pPlayer->PlaySpecialAttack(setSpecialAttack[Game::InputId::kA]);
				//次のStateを必殺技の種類に応じて変更する
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
	//上記の入力がされていなかったらStateを変更しない
	m_nextState =  shared_from_this();
}

int PlayerStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}
