#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateCharge.h"
#include "PlayerStateAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"


namespace
{
	//移動速度
	constexpr float kMoveSpeed = 100.0f;
	//トリガーが反応する
	constexpr int kTriggerReaction = 200;
	//敵との一番近い距離(真上にいかないように)
	constexpr int kShortestEnemyDistance = 1500;
	//ダッシュするときの移動速度の倍率
	constexpr float kDashSpeedRate = 1.8f;
}

void PlayerStateMove::Init()
{
	m_pPlayer->ChangeAnim("MoveFront");
}

void PlayerStateMove::Update(MyEngine::Input input)
{
	//Stateの変化
	//必殺技パレットを開いていない場合
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//一応エフェクトを消しておく
			m_pPlayer->StopEffect();
			//状態を変化させる
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX, false);
			return;
		}
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			//一応エフェクトを消しておく
			m_pPlayer->StopEffect();
			//状態を変化させる
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//ガード入力がされていたら
		if (input.IsPress(Game::InputId::kRb))
		{
			//一応エフェクトを消しておく
			m_pPlayer->StopEffect();
			//StateをGuardに変更する
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
			state->Init();
			return;
		}
		//チャージ入力されていたら
		if (input.IsPress(Game::InputId::kY))
		{
			//StateをChargeに変更する
			m_nextState = std::make_shared<PlayerStateCharge>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateCharge>(m_nextState);
			state->Init();
			return;
		}
	}
	//必殺技パレットを開いている場合
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttackId();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->StopEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->StopEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kB, true);
				return;
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->StopEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kX, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->StopEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
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
		MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
		//Y座標が関係しないようにプレイヤーと同じ座標にする
		rotationShaftPos.y = m_pPlayer->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

		//ダッシュボタンが押されているか
		bool isDash = input.IsPress(Game::InputId::kA);

		//ダッシュボタンが押されていたら視野角を広げる
		m_pPlayer->SetUpFov(isDash);


		//回転速度(横移動の速さ)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
			//ダッシュボタンを押していたら横移動の速さにダッシュの倍率をかける
			if (isDash)
			{
				hMoveSpeed *= kDashSpeedRate;
			}
		}

		m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

		//左右移動は敵の周囲を回る

		//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
		//現在の座標からその座標に向かうベクトルを作成する
		velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

		MyEngine::Vector3 zDirVec;

		//上下移動入力されていたら
		if (input.GetTriggerInfo().left > kTriggerReaction)
		{
			//前後入力を上下のベクトルに変換
			zDirVec.y += dir.z * kMoveSpeed;
		}
		//されていなかった場合
		else
		{
			//敵に近すぎたら周囲を回るようにする
			if (toShaftPosVec.Length() > kShortestEnemyDistance)
			{
				//前後入力を回転の中心に向かうベクトルに変換
				toShaftPosVec.y = 0;
				zDirVec += toShaftPosVec.Normalize() * (dir.z * kMoveSpeed);
			}
			else
			{
				//前入力を横移動に後ろ入力を回転の中心から離れるベクトルに変換
				if (dir.z > 0)
				{
					hMoveSpeed = (zDirVec.z * kMoveSpeed) / toShaftPosVec.Length();
					m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);
					zDirVec.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
					zDirVec.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;
				}
				else if (dir.z < 0)
				{
					zDirVec += toShaftPosVec.Normalize() * (dir.z * kMoveSpeed);
				}
			}
			//ダッシュボタンが押されていたら
			if (isDash)
			{
				zDirVec *= kDashSpeedRate;
				m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Dash"));
			}
			else
			{
				m_pPlayer->StopEffect();
			}
		}
		velo += zDirVec;

		//必殺技パレットが開かれていなく
		if (!input.IsPress(Game::InputId::kLb))
		{
			//回避行動の入力がされたら
			if (input.IsTrigger(Game::InputId::kA))
			{
				//一応エフェクトを消しておく
				m_pPlayer->StopEffect();
				//StateをDodgeに変更する
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
				//回避の方向を設定する
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				state->Init(dir);
				return;
			}
		}
		m_pPlayer->PlayAnim();
		m_pPlayer->SetVelo(velo);

		//正面の座標
		MyEngine::Vector3 frontPos = m_pPlayer->GetPos() + velo;
		//上下移動を行う際は敵の方を向きながら動くようにする
		if (frontPos.y != m_pPlayer->GetPos().y)
		{
			frontPos = m_pPlayer->GetTargetPos();
		}

		//移動先の座標を向くようにする
		m_pPlayer->SetModelFront(frontPos);

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
			//一応エフェクトを消しておく
			m_pPlayer->StopEffect();
			//StateをDodgeに変更する
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
			//回避の方向を設定する
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			//敵に向かうベクトルを作成する
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
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//ヒットエフェクトを表示する
	int effect = PlayEffekseer3DEffect(m_pPlayer->GetEffekseerData("Hit").first);
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	SetPosPlayingEffekseer3DEffect(effect, pos.x, pos.y, pos.z);

	return damage;
}