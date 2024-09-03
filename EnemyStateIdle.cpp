#include "EnemyStateIdle.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 30;
}

void EnemyStateIdle::Update()
{
	//���Ԃ��v������
	m_time++;

	//Idle��Ԃ͉������Ȃ�
	m_pEnemy->SetVelo(MyEngine::Vector3(0,0,0));

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();

	return damage;
}
