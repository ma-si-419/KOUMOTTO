#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackId(std::string id) { m_attackId = id; }
private:
	//経過時間
	int m_time = 0;
	//何秒間で元の状態に戻るか
	int m_actionTime = 0;
	//何の技を出しているか
	std::string m_attackId = "empty";
	//敵が近くにいるかどうか
	bool m_isNearEnemy = false;
	//攻撃の入力がされたかどうか
	bool m_isAttackInput = false;
	//通常攻撃を出している場合、何段目の攻撃を出しているか
	int m_normalAttackNum = 0;
};

