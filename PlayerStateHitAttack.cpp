#include "PlayerStateHitAttack.h"
#include "PlayerStateIdle.h"

namespace
{
	//�y���U�����󂯂��Ƃ��̃m�b�N�o�b�N�̑���
	constexpr float kLightKnockBackSpeed = 30.0f;
	//������΂��U�����󂯂����̃m�b�N�o�b�N�̑���
	constexpr float kBurstKnockBackSpeed = 120.0f;
	//�y���U�����󂯂����̓����Ȃ�����
	constexpr int kLightHitStunTime = 5;
	//������΂��U�����󂯂����̓����Ȃ�����
	constexpr int kBurstHitStunTime = 30;
	//�X�^���U�����󂯂����̓����Ȃ�����
	constexpr int kStunHitStunTime = 40;
}
void PlayerStateHitAttack::Update(MyEngine::Input input)
{
	//�o�ߎ��Ԃ��v������
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	//�y���U�����󂯂���
	if (m_hitEffect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//�y���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kLightHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}
	//������΂��U�����󂯂���
	else if (m_hitEffect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//������΂��U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kBurstHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}
	//�X�^���U�����󂯂���
	else
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//�X�^���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kStunHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}
	m_pPlayer->SetVelo(velo);
	//���݂̏�Ԃ�Ԃ�
	m_nextState = shared_from_this();
}

int PlayerStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}

void PlayerStateHitAttack::SetEffect(int effect)
{
	//�G�t�F�N�g��ۑ�����
	m_hitEffect = static_cast<HitEffect>(effect);
	//�󂯂��U���ɂ���ăA�j���[�V������ω�������
	if (m_hitEffect == HitEffect::kLightHit)
	{
		m_pPlayer->ChangeAnim("LightHit");
	}
	else if (m_hitEffect == HitEffect::kStun)
	{
		m_pPlayer->ChangeAnim("Stun");
	}
	else if (m_hitEffect == HitEffect::kBurst)
	{
		m_pPlayer->ChangeAnim("Burst");
	}
	else
	{
		m_pPlayer->ChangeAnim("Guard");
	}
}
