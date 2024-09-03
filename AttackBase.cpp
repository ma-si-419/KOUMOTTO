#include "AttackBase.h"
#include "CapsuleColliderData.h"

namespace
{
	//�G�Ƃ̋����̂����ǂ��܂Œǔ����邩
	constexpr float kTrackLengthRate = 0.9f;
	//�ǂ�����ǔ����邩
	constexpr float kTrackStartPosRate = 0.3f;
	//�ǂ̂��炢�e���U��΂点�邩
	constexpr int kScatterPower = 50;
	constexpr int kScatterPowerHalf = kScatterPower * 0.5;
}

AttackBase::AttackBase(ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_status(),
	m_dir(),
	m_isExist(true),
	m_lifeTime(0),
	m_moveLength(0),
	m_targetLength(0)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos)
{
	//�����蔻��̏��������o�^
	Collidable::Init(physics);
	//���W��ݒ肷��
	m_rigidbody.SetPos(pos);
}

void AttackBase::SetStatus(DataManager::AttackInfo status, MyEngine::Vector3 target, MyEngine::Vector3 playerPos,float power)
{
	//�R���C�_�[�f�[�^���_�E���L���X�g���f�[�^��ݒ肷��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//�X�e�[�^�X������
	m_status = status;
	//�_���[�W�͏o�����L�����̍U���͂ɔ{���������Čv�Z
	m_status.damage = static_cast<int>(m_status.damageRate * power);
	//�v���C���[����^�[�Q�b�g�Ɍ������Ă̕���������
	m_dir = (target - playerPos).Normalize();
	
	//�ł��o������������΂点��Z�ł����
	if (status.isScatter)
	{
		//X���Ƀ����_���ŉ�]������
		MATRIX randomX = MGetRotX(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);
		//Y���Ƀ����_���ŉ�]������
		MATRIX randomY = MGetRotY(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);

		MATRIX mat = MMult(randomX, randomY);

		m_dir = m_dir.MatTransform(mat);
	}
	//�����蔻��̏�������
	colData->m_radius = m_status.radius;
	colData->m_isMoveStartPos = status.isLaser;
	colData->m_startPos = m_rigidbody.GetPos() + (m_dir * status.radius);
	m_targetLength = (target - playerPos).Length();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	//�����蔻��̃f�[�^���_�E���L���X�g
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//�ړ��x�N�g�����쐬
	MyEngine::Vector3 velo = m_dir * m_status.speed;

	//�����蔻��Ɉړ��x�N�g����ݒ�
	m_rigidbody.SetVelo(velo);

	m_moveLength += velo.Length();

	//���C�t�^�C�����J�E���g����
	m_lifeTime++;

	//�G��ǔ�����U����������
	if (m_status.isTrack)
	{
		//�o�����΂���̎��͒ǔ����Ȃ��悤�ɂ���
		if (m_moveLength > m_targetLength * kTrackStartPosRate)
		{
			//�ړ��ʂ����������̓G�Ƃ̋��������Z�����
			if (m_moveLength < m_targetLength * kTrackLengthRate)
			{
				//�ǔ�����悤�ɂ���
				m_dir = (targetPos - m_rigidbody.GetPos()).Normalize();
			}
		}
	}

	//���C�t�^�C��������𒴂����珈�����Ȃ��悤�ɂ���
	if (m_lifeTime > m_status.lifeTime)
	{
		m_isExist = false;
	}
}

void AttackBase::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kPlayer)
	{
		if (GetTag() == ObjectTag::kEnemyAttack)
		{
			m_isExist = false;
		}
	}
	else if (collider->GetTag() == ObjectTag::kEnemy)
	{
		if (GetTag() == ObjectTag::kPlayerAttack)
		{
			m_isExist = false;
		}
	}
}
