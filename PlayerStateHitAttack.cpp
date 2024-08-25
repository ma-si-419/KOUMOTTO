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
void PlayerStateHitAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
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
			m_nextState = std::make_shared<PlayerStateIdle>();
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
			m_nextState = std::make_shared<PlayerStateIdle>();
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
			m_nextState = std::make_shared<PlayerStateIdle>();
			return;
		}
	}
	player->SetVelo(velo);
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
	m_nextState = std::make_shared<PlayerStateHitAttack>();
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}
