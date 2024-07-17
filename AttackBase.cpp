#include "AttackBase.h"
#include "CapsuleColliderData.h"

namespace
{
}

AttackBase::AttackBase(ObjectTag tag):
	Collidable(tag,ColliderData::Kind::kCapsule),
	m_status(),
	m_dir(),
	m_isExist(true),
	m_lifeTime(0)
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

void AttackBase::SetStatus(Game::AttackInfo status,MyEngine::Vector3 target, MyEngine::Vector3 playerPos)
{
	//コライダーデータをダウンキャストしデータを設定する
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//ステータスを入れる
	m_status = status;
	//プレイヤーからターゲットに向かっての方向を入れる
	m_dir = (target - playerPos).Normalize();
	//当たり判定の情報を入れる
	colData->m_radius = m_status.radius;
	colData->m_isMoveStartPos = status.isLaser;
	colData->m_startPos = m_rigidbody.GetPos();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	//当たり判定のデータをダウンキャスト
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//移動ベクトルを設定
	m_rigidbody.SetVelo(m_dir * m_status.speed);

	//ライフタイムをカウントする
	m_lifeTime++;

	//ライフタイムが追尾する時間だったら
	if (m_lifeTime > m_status.trackStartTime && m_lifeTime < m_status.trackEndTime)
	{
		MyEngine::Vector3 toTargetDir = (targetPos - m_rigidbody.GetPos()).Normalize();
		//方向変換が急なカーブをしないように
		//TODO:今向かっている方向からターゲットの方向に少しずつ動かしていく		    
	}

	//ライフタイムが上限を超えたら処理しないようにする
	if (m_lifeTime > m_status.lifeTime)
	{
		m_isExist = false;
	}
}
