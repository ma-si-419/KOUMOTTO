#include "AttackBase.h"

AttackBase::AttackBase() :
	m_pos(),
	m_velo()
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(MyEngine::Vector3 pos)
{
	m_pos = pos;
}

void AttackBase::Update()
{
	m_pos += m_velo;
}
