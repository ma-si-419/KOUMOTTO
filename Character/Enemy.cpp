#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
Enemy::Enemy() :
	CharacterBase("data/model/Enemy.mv1",ObjectTag::kEnemy)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Physics> physics)
{
	Collidable::Init(physics);
}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{

	//debug.x++;
	debug.y += 0.01f;
	MyEngine::Vector3 pos(sinf(debug.x) * 100, sinf(debug.y) * 3000,1000.0f);

	m_rigidbody.SetPos(pos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Enemy::Draw()
{
	DrawFormatString(800, 300, GetColor(255, 255, 255), "HP:%d\nMP:%d", m_nowHp, m_nowMp);

	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - m_status.def;
		if (damage < 0)
		{
			damage = 2;
		}

		m_nowHp -= damage;
	}
}
