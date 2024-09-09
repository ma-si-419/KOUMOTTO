#include "EnemyStateGuard.h"
#include "EffekseerForDXLib.h"
namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 120;
	//�_���[�W�̌y����
	constexpr float kDamageCutRate = 0.25f;
}
void EnemyStateGuard::Init()
{
	m_pEnemy->SetPlayEffect(m_pEnemy->GetEffekseerData("Guard"));
	m_pEnemy->ChangeAnim("Guard");
}
void EnemyStateGuard::Update()
{
	//���Ԃ��v������
	m_time++;

	m_pEnemy->PlayAnim();

	//Guard��Ԃ͈ړ����Ȃ�
	m_pEnemy->SetVelo(MyEngine::Vector3(0, 0, 0));

	

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
		m_pEnemy->StopEffect();
	}
}

int EnemyStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�J�b�g����
	damage = (attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def))) * kDamageCutRate;
	//��Ԃ�ω������Ȃ�
	return damage;
}
