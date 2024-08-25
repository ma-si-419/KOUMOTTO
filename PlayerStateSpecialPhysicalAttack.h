#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpecialPhysicalAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialPhysicalAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetTime(int time) { m_actionTime = time; }
private:
	//状態遷移して何秒目か
	int m_time = 0;
	//何秒間で元の状態に戻るか
	int m_actionTime = 0;
	//敵の近くにいるかどうか
	bool m_isNearEnemy = false;
};

