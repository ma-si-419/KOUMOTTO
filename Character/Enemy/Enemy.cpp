#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "Ui.h"
#include "EnemyStateIdle.h"
namespace
{
	//�����蔻��̑傫��
	constexpr float kColScale = 100.0f;
	//�����ʒu
	const MyEngine::Vector3 kInitPos(0, 0, 1000);
	//�X�^���Q�[�W�̃}�b�N�X
	constexpr float kMaxStanPoint = 500;
	//�X�^���Q�[�W���񕜂���܂ł̎���
	constexpr int kHealStanPointTime = 200;
	//�R���{���Ȃ���Ȃ��Ȃ�܂ł̎���
	constexpr int kComboTime = 180;
}
Enemy::Enemy() :
	CharacterBase("data/model/Enemy.mv1", ObjectTag::kEnemy),
	m_stanPoint(kMaxStanPoint),
	m_lastHitDamageTime(0),
	m_comboCount(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Physics> physics)
{

	MV1SetScale(m_modelHandle, VGet(300, 300, 300));
	Collidable::Init(physics);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rigidbody.SetPos(kInitPos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());


}

void Enemy::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	MyEngine::Vector3 pos = kInitPos;

	m_rigidbody.SetPos(pos);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//�����蔻��̏c��
	pos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = pos;
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	//State��Ai�̏����킽��
	m_pState->m_aiData = m_aiStateData;

	//�_���[�W���󂯂Ă���̎��Ԃ��J�E���g����
	m_lastHitDamageTime++;

	//�_���[�W���󂯂Ă����莞�Ԃ�������X�^���l���񕜂��Ă���
	if (m_lastHitDamageTime > kHealStanPointTime)
	{
		m_stanPoint++;
		//����l�𒴂��Ȃ��悤��
		if (m_stanPoint > kMaxStanPoint)
		{
			m_stanPoint = kMaxStanPoint;
		}
	}
	//�_���[�W���󂯂Ă����莞�Ԃ�������R���{�������炷
	if (m_lastHitDamageTime > kComboTime)
	{
		m_comboCount = 0;
	}
	//�R���{����UI�ɓn��������
	m_pUi->SetComboCount(m_comboCount);

	//State�ύX�t���O�������Ă�����State��ύX����
	if (m_pState->m_isChangeState)
	{
		m_pState = m_pState->m_nextState;
	}

	m_pState->Update();
	

	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	
	//�����蔻��̏c��
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;

	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Enemy::Draw()
{
	DrawFormatString(1300, 300, GetColor(0, 0, 0), "HP:%f\nMP:%f", m_nowHp, m_nowMp);

	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	//�v���C���[�̍U���ɓ���������
	if (collider->GetTag() == ObjectTag::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - GetRand(static_cast<int>(m_status.def));
		if (damage < 0)
		{
			damage = 2;
		}
		m_nowHp -= damage;
		if (m_nowHp < 0)
		{
			m_nowHp = 0;
		}
		//�X�^���_���[�W���󂯂�
		m_stanPoint -= attack->GetStanDamage();
		//�����肷���Ȃ��悤��
		if (m_stanPoint < 0)
		{
			m_stanPoint = 0;
		}
		//�R���{���J�E���g����^�C�~���O��������
		if (m_pUi->GetIsCountCombo())
		{
			//�R���{���𑝂₷
			m_comboCount++;
		}
		//�_���[�W���󂯂Ă���̎��Ԃ𐔂���
		m_lastHitDamageTime = 0;
		//UI�Ɏ󂯂��_���[�W�𑗂�
		m_pUi->AddShowDamage(m_rigidbody.GetPos(), damage);
	}
}

void Enemy::SetUi(std::shared_ptr<Ui> pUi)
{
	m_pUi = pUi;
}

void Enemy::InitAiState(std::shared_ptr<SceneGame> scene)
{
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), scene);
}

float Enemy::GetStanPointRate()
{
	return m_stanPoint / kMaxStanPoint;
}

void Enemy::StateUpdate(std::shared_ptr<Player> player)
{
	m_pState->CheckSituation(player);
}
