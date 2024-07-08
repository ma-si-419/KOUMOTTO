#include "AttackBase.h"

namespace
{
	constexpr int kLifeTime = 500;
}

AttackBase::AttackBase(ObjectTag tag, ColliderData::Kind kind):
	Collidable(tag,kind),
	m_atk(0),
	m_spd(0),
	m_radius(0),
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

void AttackBase::SetStatus(AttackStatus status)
{
	m_atk = status.atk;
	m_spd = status.spd;
	m_radius = status.rad;
	m_dir = status.dir;
}

void AttackBase::Update()
{
	m_lifeTime--;
	m_rigidbody.SetVelo(m_dir * m_spd);
	if (m_lifeTime < 0)
	{
		m_isExist = false;
	}
}
