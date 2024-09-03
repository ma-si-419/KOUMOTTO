#pragma once
#include "EnemyStateBase.h"
class EnemyStateMove : public EnemyStateBase
{
public:
	EnemyStateMove(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//�ǂ̍s�������邩�����߂�
	void Init(MyEngine::Vector3 playerPos);
	//�X�V����
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	enum class MoveKind
	{
		kFront,//�v���C���[�Ɍ������Ă���
		kBack,//�v���C���[���痣���
		kRight,//�E�����Ɉړ�
		kLeft//�������Ɉړ�
	};
	MoveKind m_moveKind = MoveKind::kFront;

	//�㉺�ړ����邩
	bool m_isMoveVertical = false;
};