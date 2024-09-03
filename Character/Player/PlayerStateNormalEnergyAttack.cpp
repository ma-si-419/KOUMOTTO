#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateGuard.h"
#include "Player.h"

namespace
{
	//次の攻撃に移れる時間
	constexpr int kComboTime = 40;
	//移動速度
	constexpr float kMoveSpeed = 80;
	//気弾のコンボの数
	constexpr int kComboMax = 3;
}

void PlayerStateNormalEnergyAttack::Update(MyEngine::Input input)
{
	//移動ベクトル
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//ガード入力がされていたら
	if (input.IsPress(Game::InputId::kRb))
	{
		//StateをGuardに変更する
		m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer);
		m_pPlayer->ChangeAnim("Guard");
		return;
	}

	//移動入力がされているとき
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();

		//Y軸を中心とした回転をするので
		MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
		//Y座標が関係しないようにプレイヤーと同じ座標にする
		rotationShaftPos.y = m_pPlayer->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

		//回転速度(横移動の速さ)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

		//左右移動は敵の周囲を回る

		//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
		//現在の座標からその座標に向かうベクトルを作成する
		velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

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
			MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
		}
	}
	//必殺技パレットを開いていないとき
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//時間内に攻撃入力をしていれば次段の攻撃に移行するフラグを立てる
			m_isAttackInput = true;
			//攻撃の回数がコンボの最大数を超えたら
			if (m_attackNum > kComboMax)
			{
				//コンボを一段目に戻す
				m_attackNum = 0;
			}
			m_time = 0;
		}
	}

	//経過時間管理
	m_time++;
	//入力待機時間を超えたら
	if (m_time > kComboTime)
	{
		//攻撃の入力がされていたら
		if (m_isAttackInput)
		{
			//次段の攻撃に移行する
			m_attackNum++;
			//いまだした攻撃が最終段だったら
			if (m_attackNum > kComboMax)
			{
				//アイドル状態に戻る
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}
			//攻撃の入力をリセットする
			m_isAttackInput = false;
		}
		//攻撃の入力がされていなかったら
		else
		{
			//アイドル状態に戻る
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}
	m_pPlayer->SetVelo(velo);
	//上で状態に変化がなければ今の状態を返す
	m_nextState = shared_from_this();
}

int PlayerStateNormalEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
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
