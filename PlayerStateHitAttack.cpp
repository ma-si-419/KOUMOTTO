#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//�y���U�����󂯂���
	if (m_hitEffect == HitEffect::kLightHit)
	{

	}
	//������΂��U�����󂯂���
	else if (m_hitEffect == HitEffect::kBurst)
	{

	}
	//�X�^���U�����󂯂���
	else
	{

	}
}

int PlayerStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}
