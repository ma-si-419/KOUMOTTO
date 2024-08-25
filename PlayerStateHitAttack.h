#pragma once
#include "PlayerStateBase.h"
class PlayerStateHitAttack : public PlayerStateBase
{
public:
	enum class HitEffect
	{
		kLightHit,
		kStun,
		kBurst,
		kGuard
	};
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kHitAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetEffect(int effect) { m_hitEffect = static_cast<HitEffect>(effect); }

private:
	HitEffect m_hitEffect;
};

