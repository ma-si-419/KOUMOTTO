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
	//��ԑJ�ڂ��ĉ��b�ڂ�
	int m_time = 0;
	//���b�ԂŌ��̏�Ԃɖ߂邩
	int m_actionTime = 0;
	//�G�̋߂��ɂ��邩�ǂ���
	bool m_isNearEnemy = false;
};

