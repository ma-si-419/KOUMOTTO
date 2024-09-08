#include "EnemyStateCharge.h"
#include "EffekseerForDXLib.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 120;
}

void EnemyStateCharge::Init()
{
	//�G�t�F�N�g�̐ݒ�
	m_pEnemy->SetPlayEffect(m_pEnemy->GetEffekseerData("Charge"));

	//�A�j���[�V�����̐ݒ�
	m_pEnemy->ChangeAnim("Charge");
}

void EnemyStateCharge::Update()
{
	//���Ԃ��v������
	m_time++;

	//Charge��Ԃ͒��g�I�ɂ͉������Ȃ�
	m_pEnemy->SetVelo(MyEngine::Vector3(0, 0, 0));

	//�A�j���[�V�����̍X�V
	m_pEnemy->PlayAnim();

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
		StopEffekseer3DEffect(m_playEffectHandle);
		m_pEnemy->StopEffect();
	}
}

int EnemyStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();
	StopEffekseer3DEffect(m_playEffectHandle);
	DeleteEffekseerEffect(m_effectHandle);
	m_isChangeState = true;
	return damage;
}
