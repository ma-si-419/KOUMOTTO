#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackId(std::string id) { m_attackId = id; }
private:
	//�o�ߎ���
	int m_time = 0;
	//���b�ԂŌ��̏�Ԃɖ߂邩
	int m_actionTime = 0;
	//���̋Z���o���Ă��邩
	std::string m_attackId = "empty";
	//�G���߂��ɂ��邩�ǂ���
	bool m_isNearEnemy = false;
	//�U���̓��͂����ꂽ���ǂ���
	bool m_isAttackInput = false;
	//�ʏ�U�����o���Ă���ꍇ�A���i�ڂ̍U�����o���Ă��邩
	int m_normalAttackNum = 0;
};

