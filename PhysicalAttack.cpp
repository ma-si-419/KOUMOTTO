#include "PhysicalAttack.h"

PhysicalAttack::PhysicalAttack() :
	AttackBase(ObjectTag::kPhysicalAttack, ColliderData::Kind::kSphere)
{
}

PhysicalAttack::~PhysicalAttack()
{
}

void PhysicalAttack::Draw()
{
	DrawSphere3D(m_rigidbody.GetPos().CastVECTOR(), m_radius, 8, GetColor(0, 255, 255), GetColor(255, 255, 255), false);
}
