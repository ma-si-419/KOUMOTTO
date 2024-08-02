#pragma once
#include "CharacterBase.h"
#include <memory>
class SceneGame;
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(std::shared_ptr<Physics> physics);
	void Update(std::shared_ptr<SceneGame> scene);
	void Draw();

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//スタンゲージが今全体の何割あるかを返す
	float GetStanPointRate();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
private:
	MyEngine::Vector3 debug;

	//スタンゲージ
	float m_stanPoint;
	//最後に攻撃を受けてから立った時間
	int m_lastHitDamageTime;
};
