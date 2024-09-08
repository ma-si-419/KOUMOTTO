#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
#include "CapsuleColliderData.h"
#include "PlayerStateIdle.h"
#include "EffekseerForDXLib.h"
namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	//�U�����o�����W
	constexpr float kAttackPos = 500.0f;
	//�i���U�����o�����̓G�Ƃ̋���(�����蔻��̑傫���̔{��)
	constexpr float kPhysicalAttackLange = 0.9f;
	//�����蔻��̑傫��
	constexpr float kColScale = 100.0f;
	//�g���K�[����������
	constexpr int kTriggerReaction = 200;
	//������]�x
	constexpr int kInitRota = 10;
	//���f���̊g�嗦
	constexpr float kModelScale = 3.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0),
	m_lastAttackTime(0),
	m_isOpenSpecialPallet(false)
{
	m_playEffectData.first = -1;
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	SetSpecialAttack();

	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = kInitRota;

	m_pPhysics = physics;
	Collidable::Init(physics);

	//Y���𒆐S�Ƃ�����]������̂�
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	m_rigidbody.SetPos(pos);

	//�����蔻��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;
	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//�G�̕����������悤�ɂ���
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Player::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = kInitRota;


	//Y���𒆐S�Ƃ�����]������̂�
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	//�����蔻��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);

	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;

	m_rigidbody.SetPos(pos);

	//State�̏�����
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//�G�̕����������悤�ɂ���
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//�O�̃t���[����State���r���ĈႤState��������
	if (m_pState->m_nextState->GetKind() != m_pState->GetKind())
	{
		m_isUpFov = false;
		//State��ύX����
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}
#ifdef _DEBUG
	std::string stateKind = "empty";

	if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kAttack)
	{
		stateKind = "Attack\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kCharge)
	{
		stateKind = "Charge\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kDodge)
	{
		stateKind = "Dodge\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kGuard)
	{
		stateKind = "Guard\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		stateKind = "HitAttack\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kIdle)
	{
		stateKind = "Idle\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kMove)
	{
		stateKind = "Move\n";
	}

	printfDx(stateKind.c_str());
#endif // _DEBUG

	m_isOpenSpecialPallet = input.IsPress(Game::InputId::kLb);

	//State�̍X�V
	m_pState->Update(input);

	//�G�t�F�N�g�̍Đ�
	PlayEffect();

	//�����蔻��̍X�V
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//�����蔻��̏c��
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;
	//�n���h���̍��W��ݒ肷��
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//�̗͂�0�ȉ��ɂȂ�ƃQ�[���I�[�o�[
	if (m_nowHp <= 0)
	{
		scene->GameOver();
	}
#ifdef _DEBUG
	if (input.IsPress(Game::InputId::kSelect))
	{
		m_nowHp -= 1000;
	}
#endif
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

bool Player::GetAttackKind(std::string attackId)
{
	return m_attackData[attackId].isEnergy;
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = m_pState->OnDamage(collider);
		if (damage < 0)
		{
			damage = 2;
		}

		m_nowHp -= damage;
	}
}

std::map<std::string, std::string> Player::GetSetSpecialAttackName()
{
	std::map<std::string, std::string> ans;

	ans[Game::InputId::kA] = m_attackData[m_setSpecialAttackId[Game::InputId::kA]].name;
	ans[Game::InputId::kB] = m_attackData[m_setSpecialAttackId[Game::InputId::kB]].name;
	ans[Game::InputId::kX] = m_attackData[m_setSpecialAttackId[Game::InputId::kX]].name;
	ans[Game::InputId::kY] = m_attackData[m_setSpecialAttackId[Game::InputId::kY]].name;

	return ans;
}

std::shared_ptr<AttackBase> Player::CreateAttack(std::string id)
{
	//�G�l�~�[�̍U�����쐬
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);

	//�U�����o�����W���쐬

	MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//�X�e�[�^�X�ݒ�
	ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos(), m_status.atk);
	ans->Init(m_pPhysics, attackPos, m_effekseerHandle[m_attackData[id].effekseerName].first);

	return ans;
}

void Player::SetPlayEffect(std::pair<int, int> playHandleData)
{
	m_playEffectData = playHandleData;
}

void Player::StopEffect()
{
	StopEffekseer3DEffect(m_playingEffectHandle);
	m_playEffectData.first = -1;
	m_playEffectData.second = 0;
	m_playingEffectHandle = -1;
	m_playEffectFrame = 0;
}

void Player::ChangeState(std::shared_ptr<PlayerStateBase> state)
{
	m_pState = state;
}

void Player::SetSpecialAttack()
{
	m_setSpecialAttackId["A"] = CommandId::kSpEnergyAttack;
	m_setSpecialAttackId["B"] = CommandId::kSpStanAttack;
	m_setSpecialAttackId["X"] = CommandId::kSpSlamAttack;
	m_setSpecialAttackId["Y"] = CommandId::kSpLaserAttack;

}
