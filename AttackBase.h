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
	/*ステータス*/
	//攻撃力
	int m_atk;
	//速度
	float m_spd;
	//攻撃の方向
	MyEngine::Vector3 m_dir;
	
	//存在しているか
	bool m_isExist;
	//存在する時間
	int m_lifeTime;
	//当たり判定が球かどうか
	bool m_isSphere;
};

