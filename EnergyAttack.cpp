#include "EnergyAttack.h"

EnergyAttack::EnergyAttack():
	AttackBase(ObjectTag::kEnergyAttack,ColliderData::Kind::kSphere)
{
}

EnergyAttack::~EnergyAttack()
{
}


void EnergyAttack::Draw()
{
	DrawSphere3D(m_rigidbody.GetPos().CastVECTOR(), m_radius, 8, GetColor(255, 0, 0), GetColor(255, 255, 255), false);
}
