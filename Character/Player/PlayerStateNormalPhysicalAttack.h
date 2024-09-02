#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalPhysicalAttack : public PlayerStateBase
{
public:
	PlayerStateNormalPhysicalAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalPhysicalAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//�����i�ڂ̍U�������Ă���̂�
	int m_attackNum = 0;
	//��ԑJ�ڂ��ĉ��b�ڂ�
	int m_time = 0;
	//�U���̓��͂����ꂽ���ǂ���
	bool m_isAttackInput = false;
	//�G�̋߂��ɂ��邩�ǂ���
	bool m_isNearEnemy = false;
};

