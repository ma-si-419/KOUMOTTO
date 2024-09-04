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
	m_effectHandle = LoadEffekseerEffect("data/effekseer/barrior.efk");
	m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle);
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	SetPosPlayingEffekseer3DEffect(m_playEffectHandle,pos.x,pos.y,pos.z);
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
		StopEffekseer3DEffect(m_playEffectHandle);
		DeleteEffekseerEffect(m_effectHandle);
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
