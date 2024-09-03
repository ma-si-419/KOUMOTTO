#include "EnemyStateAttack.h"
#include "CommandIdList.h"

namespace
{
	//格闘攻撃を優先する距離
	constexpr float kPrioritizePhysialAttackDistance = 3000.0f;

	//気弾攻撃を優先する距離
	constexpr float kPrioritizeEnergyAttackDistance = 10000.0f;

	//格闘攻撃を出す距離
	constexpr float kPhysicalAttackDistance = 1500.0f;

	//格闘攻撃を出すときの移動速度
	constexpr float kMoveSpeed = 100.0f;

	//基本的な攻撃方法の割合
	constexpr int kAttackKindRate[4] = { 30,30,20,20 };
	//格闘攻撃を優先するときの割合
	constexpr int kPrioritizePhysialAttackKindRate[4] = { 20,20,30,30 };
	//気弾攻撃を優先するときの割合
	constexpr int kPrioritizeEnergyAttackKindRate[4] = { 50,50,0,0 };

}

void EnemyStateAttack::Init(std::shared_ptr<Player> player)
{
	//初期化
	m_time = 0;
	m_isNearPlayer = true;
	m_pPlayer = player;


	//行動をなににするか
	int attackKind = 0;

	//距離によって出す技の優先度を変化させる
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizeEnergyAttackDistance)
	{
		//気弾攻撃が優先される場合
		int totalRate = 0;
		//確率をすべて足す
		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizePhysialAttackDistance)
	{
		//格闘攻撃が優先される場合
		int totalRate = 0;
		//確率をすべて足す
		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else
	{
		//どちらも優先しない場合
		int totalRate = 0;
		//確率をすべて足す
		for (auto item : kAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	//攻撃の種類を見て攻撃を決定する
	if (attackKind == static_cast<int>(AttackKind::kRepeatedlyEnergy))
	{
		m_attackId = CommandId::kSpEnergyAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kLaser))
	{
		m_attackId = CommandId::kSpLaserAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kBreakAttack))
	{
		m_attackId = CommandId::kSpSlamAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}
	else if (attackKind == static_cast<int>(AttackKind::kStanAttack))
	{
		m_attackId = CommandId::kSpStanAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}
}

void EnemyStateAttack::Update()
{
	m_time++;
	//移動ベクトル
	MyEngine::Vector3 velo;

	//敵に向かっていく
	if (m_isNearPlayer)
	{
		velo = (m_pPlayer->GetPos() - m_pEnemy->GetPos()).Normalize() * kMoveSpeed;
	}
	//一定距離まで近づいたら
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() < kPhysicalAttackDistance)
	{
		//動きを止める
		velo = MyEngine::Vector3(0, 0, 0);
		//攻撃を出し始める
		m_isStartAttack = true;
	}
	//攻撃を出している時間を計測する
	if (m_isStartAttack)
	{
		m_attackTime++;
	}

	m_pEnemy->SetVelo(velo);

	//攻撃の情報を取得する
	std::map<std::string, DataManager::AttackInfo> attackData = m_pEnemy->GetAttackData();

	//攻撃を出し始めて何フレームかを見て攻撃の処理を行う
	if (m_attackTime > attackData[m_attackId].attackStartTime)
	{
		//攻撃を複数回出す技であれば
		if (attackData[m_attackId].attackNum > 1)
		{
			//攻撃のスパンを取得する
			int span = (attackData[m_attackId].attackEndTime - attackData[m_attackId].attackStartTime) / attackData[m_attackId].attackNum;
			//攻撃のタイミングが来たら攻撃を出すようにする
			if (m_attackTime % span == 0 && m_attackTime < attackData[m_attackId].attackEndTime)
			{
				//攻撃を作成
				std::shared_ptr<AttackBase> attack = m_pEnemy->CreateAttack(m_attackId);
				//レーザー状の攻撃であれば消える時間をそろえる
				if (attackData[m_attackId].isLaser)
				{
					//消えるまでの時間
					int lifeTime = attackData[m_attackId].lifeTime - m_attackTime;

					attack->SetAttackTime(lifeTime);
				}
				//攻撃を出す
				m_pScene->AddAttack(attack);
			}
		}
		//単発攻撃であれば
		else
		{
			//攻撃を出す時間になったら
			if (m_attackTime > attackData[m_attackId].attackStartTime)
			{
				//攻撃を出す
				m_pScene->AddAttack(m_pEnemy->CreateAttack(m_attackId));
			}

		}

	}
	//攻撃の時間が終わったら
	if (m_attackTime > attackData[m_attackId].attackEndTime)
	{
		//べつのStateに移動する
		m_isChangeState = true;
	}
}
int EnemyStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//ダメージは食らうが、Stateを変更しない

	return damage;
}
