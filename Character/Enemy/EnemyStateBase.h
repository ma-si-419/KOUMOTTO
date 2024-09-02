#pragma once
#include "Enemy.h"
#include "SceneGame.h"
#include "PlayerStateBase.h"

class EnemyStateBase
{
public:

	enum class EnemyStateKind
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kAttack,
		kGuard,
		kCharge,
		kHitAttack
	};
	EnemyStateBase(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) { m_pEnemy = enemy; m_pScene = scene; }
	//�󋵂�c����������X�V
	void CheckSituation(PlayerStateBase::PlayerStateKind playerState);
	//�v���C���[��State�����Ď��̓��������肷��
	virtual void Update() abstract;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//���ڍs����State��ۑ�����
	std::shared_ptr<PlayerStateBase> m_nextState;
	//�G�l�~�[�̃|�C���^�[�������Ă���
	std::shared_ptr<Enemy> m_pEnemy;
	//�Q�[���V�[���̃|�C���^�[�������Ă���
	std::shared_ptr<SceneGame> m_pScene;
};

