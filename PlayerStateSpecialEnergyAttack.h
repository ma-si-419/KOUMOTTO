#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpecialEnergyAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetTime(int time) { m_actionTime = time; }
private:
	//何秒間で元の状態に戻るか
	int m_actionTime = 0;
};