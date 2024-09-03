#include "EnemyStateHitAttack.h"

void EnemyStateHitAttack::Update()
{
	m_isChangeState = true;
}

int EnemyStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}
