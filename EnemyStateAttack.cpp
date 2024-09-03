#include "EnemyStateAttack.h"

void EnemyStateAttack::Update()
{
	m_isChangeState = true;

}

int EnemyStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}
