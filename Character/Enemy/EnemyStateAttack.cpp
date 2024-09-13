#include "EnemyStateAttack.h"
#include "CommandIdList.h"
#include "EffekseerForDXLib.h"

namespace
{
	//格闘攻撃を優先する距離
	constexpr float kPrioritizePhysialAttackDistance = 300.0f;

	//気弾攻撃を優先する距離
	constexpr float kPrioritizeEnergyAttackDistance = 1000.0f;

	//格闘攻撃を出す距離
	constexpr float kPhysicalAttackDistance = 150.0f;

	//格闘攻撃を出すときの移動速度
	constexpr float kMoveSpeed = 15.0f;

	//格闘攻撃を離れていても出す時間
	constexpr int kMaxMoveTime = 120;

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
		m_attackId = CommandId::kEnemyEnergyAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kLaser))
	{
		m_attackId = CommandId::kEnemyLaserAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kBreakAttack))
	{
		m_attackId = CommandId::kEnemySlamAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}
	else if (attackKind == static_cast<int>(AttackKind::kStanAttack))
	{
		m_attackId = CommandId::kEnemyStanAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}

	m_pEnemy->ChangeAnim(m_pEnemy->GetAttackData()[m_attackId].animationName);
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
	//一定距離まで近づくか一定時間たったら
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() < kPhysicalAttackDistance || m_time > kMaxMoveTime)
	{
		//動きを止める
		velo = MyEngine::Vector3(0, 0, 0);
		//攻撃を出し始める
		m_isStartAttack = true;
		//敵に近づくのをやめる
		m_isNearPlayer = false;
	}
	//攻撃を出している時間を計測する
	if (m_isStartAttack)
	{
		m_attackTime++;
	}

	m_pEnemy->SetVelo(velo);

	//攻撃の情報を取得する
	std::map<std::string, DataManager::AttackInfo> attackData = m_pEnemy->GetAttackData();

	//攻撃が終わっていない時に
	if (!m_isAttackEnd)
	{
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
					//レーザーを出す座標が指定されていなかったら
					if (m_laserTargetPos.sqLength() == 0)
					{
						m_laserTargetPos = m_pEnemy->GetTargetPos();
					}
					//攻撃を作成
					std::shared_ptr<AttackBase> attack = m_pEnemy->CreateAttack(m_attackId, m_laserTargetPos);
					//レーザー状の攻撃の設定
					if (attackData[m_attackId].isLaser)
					{
						//消えるまでの時間
						int lifeTime = attackData[m_attackId].lifeTime - m_attackTime;

						attack->SetAttackTime(lifeTime);
						//エフェクトを残すようにする
						attack->SetLeaveEffect();
						//一度目の攻撃以外エフェクトを出さないようにする
						if (m_popAttackNum != 0)
						{
							attack->SetNotPopEffect();
						}

						m_popAttackNum++;
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
					m_pScene->AddAttack(m_pEnemy->CreateAttack(m_attackId, MyEngine::Vector3(0, 0, 0)));
					m_isAttackEnd = true;
				}
			}
		}
	}
	//攻撃の時間が終わったら
	if (m_attackTime > attackData[m_attackId].attackEndTime && !m_isAttackEndStanAnim)
	{
		m_isAttackEndStanAnim = true;
		m_isAttackEnd = true;
		m_pEnemy->SetAttackEndAnim(attackData[m_attackId].actionTotalTime - attackData[m_attackId].attackEndTime);
	}
	//行動全体の時間が終わったら
	if (m_attackTime > attackData[m_attackId].actionTotalTime)
	{
		m_isChangeState = true;
	}

	m_pEnemy->PlayAnim();

}
int EnemyStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//基本Stateは変更しないが、吹き飛ばしだけ変化を受ける
	if (attack->GetHitEffect() == static_cast<int>(EnemyStateBase::HitEffectKind::kBurst))
	{
		m_hitEffect = attack->GetHitEffect();
		m_isChangeState = true;
	}
	int effect = PlayEffekseer3DEffect(m_pEnemy->GetEffekseerData("Hit").first);
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	SetPosPlayingEffekseer3DEffect(effect, pos.x, pos.y, pos.z);
	return damage;
}
