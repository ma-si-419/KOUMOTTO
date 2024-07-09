#include "AttackBase.h"
#include "CapsuleColliderData.h"

namespace
{
	constexpr int kLifeTime = 500;
}

AttackBase::AttackBase(ObjectTag tag):
	Collidable(tag,ColliderData::Kind::kCapsule),
	m_atk(0),
	m_spd(0),
	m_dir(),
	m_isExist(true),
	m_lifeTime(kLifeTime)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(Physics* physics, MyEngine::Vector3 pos)
{
	Collidable::Init(physics);
	m_rigidbody.SetPos(pos);
}

void AttackBase::SetStatus(Game::AttackInfo status,MyEngine::Vector3 target)
{
	auto colData = dynamic_cast<CapsuleColliderData*>(Collidable::m_pColData);
	m_atk = status.damageRate;
	m_spd = status.speed;
	colData->m_radius = status.radius;
	m_dir = target;
	colData->m_isMoveStartPos = status.isLaser;
	colData->m_startPos = m_rigidbody.GetPos();
}

void AttackBase::Update()
{
	auto colData = dynamic_cast<CapsuleColliderData*>(Collidable::m_pColData);

	m_rigidbody.SetVelo(m_dir * m_spd);
	m_lifeTime--;
	if (m_lifeTime < 0)
	{
		m_isExist = false;
	}
}
