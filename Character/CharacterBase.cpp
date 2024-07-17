#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0)
{
	m_modelHandle = MV1LoadModel(model);
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id)
{
	std::shared_ptr<AttackBase> ans;
	//攻撃データの種類を取得
	if (m_attackData[id].isEnergy)
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kEnergyAttack);
	}
	else
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kPhysicalAttack);
	}
	//攻撃を出す座標を作成
	MyEngine::Vector3 toEnemyVec = m_targetPos - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toEnemyVec.Normalize() * m_attackData[id].radius;

	ans->Init(physics, attackPos);
	//ステータス設定
	ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos());

	return ans;
}

void CharacterBase::SetAttack(std::string id)
{
	//技で動けない時間を設定する
	m_stanTime = m_attackData[id].actionTime;
	//攻撃を出している状態に変える
	m_isAttack = true;
	//出している攻撃の種類を取得する
	m_attackId = id;
}
