#pragma once
#include "EnemyStateBase.h"
class EnemyStateMove : public EnemyStateBase
{
public:
	EnemyStateMove(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//�v���C���[��State�����Ď��̓��������肷��
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};