#pragma once
#include "Collidable.h"
#include "Game.h"
class AttackBase : public Collidable
{
public:

	AttackBase(ObjectTag tag);
	virtual ~AttackBase();

	void Init(std::shared_ptr<Physics> physics,MyEngine::Vector3 pos);
	void SetStatus(Game::AttackInfo status,MyEngine::Vector3 target, MyEngine::Vector3 playerPos,float power);
	void Update(MyEngine::Vector3 targetPos);
	void Draw() {};

	//�_���[�W��Ԃ�
	int GetDamage() {return m_status.damage;}

	//�����ɓ����������̏���
	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//���C�t�^�C����ݒ肷��
	void SetAttackTime(int time) { m_status.lifeTime = time; };

	void Final(std::shared_ptr<Physics> physics) { Collidable::Final(physics); }
	bool GetIsExist() { return m_isExist; }
protected:
	/*�X�e�[�^�X*/
	Game::AttackInfo m_status;
	//�U���̕���
	MyEngine::Vector3 m_dir;
	//���݂��Ă��邩
	bool m_isExist;
	//���݂�������
	int m_lifeTime;
	//���������̓G�Ƃ̋���
	float m_targetLength;
	//�ړ���������
	float m_moveLength;
};

