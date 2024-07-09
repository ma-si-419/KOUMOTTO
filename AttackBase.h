#pragma once
#include "Collidable.h"
#include "Game.h"
class AttackBase : public Collidable
{
public:

	AttackBase(ObjectTag tag);
	virtual ~AttackBase();

	void Init(Physics* physics,MyEngine::Vector3 pos);
	void SetStatus(Game::AttackInfo status,MyEngine::Vector3 target);
	void Update();
	virtual void Draw() = 0;

	bool GetExist() { return m_isExist; }
protected:
	/*�X�e�[�^�X*/
	//�U����
	int m_atk;
	//���x
	float m_spd;
	//�U���̕���
	MyEngine::Vector3 m_dir;
	
	//���݂��Ă��邩
	bool m_isExist;
	//���݂��鎞��
	int m_lifeTime;
	//�����蔻�肪�����ǂ���
	bool m_isSphere;
};

