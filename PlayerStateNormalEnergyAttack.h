#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalEnergyAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackNum(int num) { m_attackNum = num; }
private:
	//�����i�ڂ̍U�������Ă���̂�
	int m_attackNum = 0;
	//��ԑJ�ڂ��ĉ��b�ڂ�
	int m_time = 0;
	//�U���̓��͂����ꂽ���ǂ���
	bool m_isAttackInput = false;
};

