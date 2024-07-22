#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
namespace
{
	//“–‚½‚è”»’è‚Ì‘å‚«‚³
	constexpr float kColScale = 100.0f;
}
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
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{

	//debug.x++;
	debug.y += 0.01f;
	MyEngine::Vector3 pos(sinf(debug.x) * 100, sinf(debug.y) * 3000,1000.0f);

	m_rigidbody.SetPos(pos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//“–‚½‚è”»’è‚Ìc•
	pos.y += kColScale;
	//“–‚½‚è”»’è‚ÌÀ•W’²®
	colData->m_startPos = pos;

}

void Enemy::Draw()
{
	DrawFormatString(800, 300, GetColor(255, 255, 255), "HP:%d\nMP:%d", m_nowHp, m_nowMp);

	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	//ƒvƒŒƒCƒ„[‚ÌUŒ‚‚É“–‚½‚Á‚½Žž
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
