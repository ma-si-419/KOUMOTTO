#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"


namespace
{
	//攻撃を出しているときの移動速度
	constexpr float kMoveSpeed = 150;
	//敵が近くにいる判定になる距離
	constexpr float kNearEnemyLength = 200;
	//敵に近づく時間の最大
	constexpr int kGoEnemyTime = 60;
}

void PlayerStateSpecialPhysicalAttack::Update(MyEngine::Input input)
{
	m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
	return;
	//経過時間を計測する
	m_time++;

	//移動ベクトルの生成
	MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
	MyEngine::Vector3 velo = moveVec.Normalize() * kMoveSpeed;

	//敵が近くにいるかどうかを調べる
	float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
	//敵が近くにいるか、経過時間が一定時間を超えたら
	if (length < kNearEnemyLength || m_time > kGoEnemyTime)
	{
		//敵が近くにいるフラグを立てる
		m_isNearEnemy = true;
	}
	//敵が近くにいたら移動をやめて攻撃を出す
	if (m_isNearEnemy)
	{
		//移動をやめる処理
		velo = MyEngine::Vector3(0, 0, 0);
		//TODO : 攻撃を行う処理を作る
		//攻撃の処理をする時間をへらしていく
		m_actionTime--;
		if (m_actionTime < 0)
		{
			//攻撃の処理をする時間が終わったら
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		}
		
	}

	//移動ベクトルを入れる
	m_pPlayer->SetVelo(velo);
	m_nextState = shared_from_this();
}

int PlayerStateSpecialPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}
