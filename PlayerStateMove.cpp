#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateHitAttack.h"

namespace
{
	//移動速度
	constexpr float kMoveSpeed = 100.0f;
	//ダメージの軽減率
	constexpr float kDamageCutRate = 0.25f;

}

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

	//移動ベクトル
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//移動入力がされているとき
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();

		//Y軸を中心とした回転をするので
		MyEngine::Vector3 rotationShaftPos = player->GetTargetPos();
		//Y座標が関係しないようにプレイヤーと同じ座標にする
		rotationShaftPos.y = player->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - player->GetPos();

		//回転速度(横移動の速さ)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		MyEngine::Vector3 a = rotationShaftPos - player->GetPos();

		//m_rota = atan2f(a.z,a.x);

		player->SetRota(player->GetRota() + hMoveSpeed);

		//左右移動は敵の周囲を回る

		//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
		//現在の座標からその座標に向かうベクトルを作成する
		velo.x = (rotationShaftPos.x + cosf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().z;

		//上下移動入力されていたら
		if (input.IsPress(Game::InputId::kLb))
		{
			//前後入力を上下のベクトルに変換
			velo.y += dir.z * kMoveSpeed;
		}
		//されていなかった場合
		else
		{
			//前後入力を回転の中心に向かうベクトルに変換
			MyEngine::Vector3 toCenterVec = player->GetTargetPos() - player->GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
		}

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
			state->SetMoveDir(dir);
			return;
		}

		player->MoveAnim(dir);
		player->SetVelo(velo);

		//自分のポインタを返す
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
			state->SetMoveDir(dir.Normalize());
			return;
		}
		//何も入力がされていなければ
		m_nextState = std::make_shared<PlayerStateIdle>();
	}

}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージを三分の一にして返す
	damage = attack->GetDamage() * kDamageCutRate;
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>();
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(static_cast<int>(PlayerStateHitAttack::HitEffect::kGuard));

	return damage;
}
