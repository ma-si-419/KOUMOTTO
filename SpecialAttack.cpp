#include "SpecialAttack.h"

SpecialAttack::SpecialAttack(ObjectTag tag, ColliderData::Kind kind):
	AttackBase(tag,kind)
{
}

SpecialAttack::~SpecialAttack()
{
}

void SpecialAttack::Draw()
{
	DrawSphere3D(m_rigidbody.GetPos().CastVECTOR(), m_radius, 8, GetColor(255, 0, 255), GetColor(255, 255, 255), false);
}
