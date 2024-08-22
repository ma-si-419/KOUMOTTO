#pragma once
#include "PlayerStateBase.h"
class PlayerStateHitAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kHitAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetEffect(int effect) { m_hitEffect = static_cast<HitEffect>(effect); }

private:
	enum class HitEffect
	{
		kLightHit,
		kStun,
		kBurst
	};
	HitEffect m_hitEffect;
};

