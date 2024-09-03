#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	//攻撃を出しているときの移動速度
	constexpr float kMoveSpeed = 120;
	//敵が近くにいる判定になる距離
	constexpr float kNearEnemyLength = 200;
	//敵に近づく時間の最大
	constexpr int kGoEnemyTime = 60;
	//攻撃の最大コンボ数
	constexpr int kMaxCombo = 2;
}

void PlayerStateNormalPhysicalAttack::Update(MyEngine::Input input)
{
	m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
	return;

	//移動ベクトルの生成
	MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
	MyEngine::Vector3 velo = moveVec.Normalize() * kMoveSpeed;

	//必殺技パレットを開いていないとき
	if (!input.IsPress(Game::InputId::kLb))
	{
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			m_isAttackInput = true;
		}
	}

	//敵が近くにいるかどうかを調べる
	float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
	if (length < kNearEnemyLength)
	{
		m_isNearEnemy = true;
	}
	//敵が近くにいたら移動をやめて攻撃を出す
	if (m_isNearEnemy)
	{
		//移動をやめる処理
		velo = MyEngine::Vector3(0, 0, 0);
		//TODO : 攻撃を行う処理を作る
	}
	//状態遷移して何秒か測る
	m_time++;
	//敵に向かう時間が一定値を超えたら
	if (m_time > kGoEnemyTime)
	{
		//TODO : 攻撃を行う処理を作る

		//攻撃入力を行っていたら
		if (m_isAttackInput)
		{
			//時間をリセットし
			m_time = 0;
			//攻撃を次の段に移行する
			m_attackNum++;
			//攻撃の最大数を超えていたら
			if (m_attackNum > kMaxCombo)
			{
				//アイドル状態に戻す
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}
			//敵が近くにいるフラグをfalseにする
			m_isNearEnemy = false;
		}
		//攻撃入力がされていなかったら
		else
		{
			//アイドル状態に戻す
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}

	//移動ベクトルを入れる
	m_pPlayer->SetVelo(velo);
	//上で状態の変化がなかったら今の状態を返す
	m_nextState = shared_from_this();
}

int PlayerStateNormalPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
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
