#pragma once
#include "Collidable.h"
#include "Game.h"
class AttackBase : public Collidable
{
public:

	AttackBase(ObjectTag tag);
	virtual ~AttackBase();

	void Init(std::shared_ptr<Physics> physics,MyEngine::Vector3 pos);
	void SetStatus(Game::AttackInfo status,MyEngine::Vector3 target, MyEngine::Vector3 playerPos);
	void Update(MyEngine::Vector3 targetPos);
	virtual void Draw() = 0;

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
};

