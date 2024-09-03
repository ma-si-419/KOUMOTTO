#include "AttackBase.h"
#include "CapsuleColliderData.h"

namespace
{
	//敵との距離のうちどこまで追尾するか
	constexpr float kTrackLengthRate = 0.9f;
	//どこから追尾するか
	constexpr float kTrackStartPosRate = 0.3f;
	//どのくらい弾を散らばらせるか
	constexpr int kScatterPower = 50;
	constexpr int kScatterPowerHalf = kScatterPower * 0.5;
}

AttackBase::AttackBase(ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_status(),
	m_dir(),
	m_isExist(true),
	m_lifeTime(0),
	m_moveLength(0),
	m_targetLength(0)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos)
{
	//当たり判定の初期化＆登録
	Collidable::Init(physics);
	//座標を設定する
	m_rigidbody.SetPos(pos);
}

void AttackBase::SetStatus(DataManager::AttackInfo status, MyEngine::Vector3 target, MyEngine::Vector3 playerPos,float power)
{
	//コライダーデータをダウンキャストしデータを設定する
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//ステータスを入れる
	m_status = status;
	//ダメージは出したキャラの攻撃力に倍率をかけて計算
	m_status.damage = static_cast<int>(m_status.damageRate * power);
	//プレイヤーからターゲットに向かっての方向を入れる
	m_dir = (target - playerPos).Normalize();
	
	//打ち出す方向をちらばらせる技であれば
	if (status.isScatter)
	{
		//X軸にランダムで回転させる
		MATRIX randomX = MGetRotX(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);
		//Y軸にランダムで回転させる
		MATRIX randomY = MGetRotY(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);

		MATRIX mat = MMult(randomX, randomY);

		m_dir = m_dir.MatTransform(mat);
	}
	//当たり判定の情報を入れる
	colData->m_radius = m_status.radius;
	colData->m_isMoveStartPos = status.isLaser;
	colData->m_startPos = m_rigidbody.GetPos() + (m_dir * status.radius);
	m_targetLength = (target - playerPos).Length();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	//当たり判定のデータをダウンキャスト
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//移動ベクトルを作成
	MyEngine::Vector3 velo = m_dir * m_status.speed;

	//当たり判定に移動ベクトルを設定
	m_rigidbody.SetVelo(velo);

	m_moveLength += velo.Length();

	//ライフタイムをカウントする
	m_lifeTime++;

	//敵を追尾する攻撃だったら
	if (m_status.isTrack)
	{
		//出したばかりの時は追尾しないようにする
		if (m_moveLength > m_targetLength * kTrackStartPosRate)
		{
			//移動量が初期化時の敵との距離よりも短ければ
			if (m_moveLength < m_targetLength * kTrackLengthRate)
			{
				//追尾するようにする
				m_dir = (targetPos - m_rigidbody.GetPos()).Normalize();
			}
		}
	}

	//ライフタイムが上限を超えたら処理しないようにする
	if (m_lifeTime > m_status.lifeTime)
	{
		m_isExist = false;
	}
}

void AttackBase::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kPlayer)
	{
		if (GetTag() == ObjectTag::kEnemyAttack)
		{
			m_isExist = false;
		}
	}
	else if (collider->GetTag() == ObjectTag::kEnemy)
	{
		if (GetTag() == ObjectTag::kPlayerAttack)
		{
			m_isExist = false;
		}
	}
}
