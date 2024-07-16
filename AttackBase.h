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
	/*ステータス*/
	Game::AttackInfo m_status;
	//攻撃の方向
	MyEngine::Vector3 m_dir;
	//存在しているか
	bool m_isExist;
	//存在した時間
	int m_lifeTime;
};

