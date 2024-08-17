#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"
#include "CommandIdList.h"
#include "LoadCsv.h"

namespace
{
	//�i���U����A���ŏo�������ǂ����𔻒f���鎞��
	constexpr int kPhysicalAttackConnectTime = 30;
	//�C�e�U����A���ŏo�������ǂ����𔻒f���鎞��
	constexpr int kEnergyAttackConnectTime = 30;
}

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0),
	m_isAttack(false),
	m_animTime(0),
	m_animPlaySpeed(0),
	m_totalAnimTime(0)
{
	m_modelHandle = MV1LoadModel(model);
	//auto& coldata = std::dynamic_pointer_cast<CapsuleColliderData>;
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

void CharacterBase::LoadAnimationData(bool isPlayer)
{
	LoadCsv loadCsv;
	std::vector<std::vector<std::string>> data;
	if (isPlayer)
	{
		//�v���C���[�̃t�@�C����ǂݍ���
		data = loadCsv.LoadFile("data/animationData.csv");
	}
	else
	{
		//�G�l�~�[�̃t�@�C����ǂݍ���
	}

	for (auto& item : data)
	{
		//�����f�[�^
		AnimationInfo pushData;

		pushData.number = std::stoi(item[static_cast<int>(AnimationInfoSort::kNumber)]);
		pushData.startFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kStartFrame)]);
		pushData.loopFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kLoopFrame)]);
		pushData.endFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kEndFrame)]);
		pushData.playSpeed = std::stof(item[static_cast<int>(AnimationInfoSort::kPlaySpeed)]);

		m_animData[item[static_cast<int>(AnimationInfoSort::kName)]] = pushData;
	}

}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer)
{
	std::shared_ptr<AttackBase> ans;
	//�U���f�[�^�̎�ނ��擾
	if (isPlayer)
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	}
	else
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	}
	//�U�����o�����W���쐬
	MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	ans->Init(physics, attackPos);
	//�X�e�[�^�X�ݒ�
	ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);

	return ans;
}

void CharacterBase::ChangeAnim(std::string animName)
{

	//�O�̃A�j���[�V�������f�^�b�`����
	for (auto item : m_playAnims)
	{
		MV1DetachAnim(m_modelHandle, item.first);
	}
	//�Đ����Ă����A�j���[�V�����̔z����폜����
	m_playAnims.clear();
	//�A�j���̍Đ����x��ݒ�
	m_animPlaySpeed = m_animData[animName].playSpeed;
	//�A�j���̍Đ����Ԃ����Z�b�g
	m_animTime = 0;
	//�V�����A�j���[�V�������A�^�b�`����
	std::pair<int, int> pushData;
	pushData.first = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	pushData.second = m_animData[animName].number;
	m_playAnims.push_back(pushData);
	//�A�j���[�V�����̑��Đ����Ԃ�ݒ肷��
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].endFrame);
}

void CharacterBase::MoveAnim(MyEngine::Vector3 moveDir)
{
	std::vector<std::pair<int, int>> playAnims;

	//�X�e�B�b�N�̌X���ɕ����ăA�j���[�V������ǉ�����
	if (moveDir.x > 0.0f)
	{
		std::pair<int, int> pushData;
		pushData.first = m_playAnims.size();
		pushData.second = m_animData["MoveRight"].number;
		playAnims.push_back(pushData);
		//MV1SetAttachAnimBlendRate(m_modelHandle, pushData.first, moveDir.x);
	}
	else if (moveDir.x < 0.0f)
	{
		std::pair<int, int> pushData;
		pushData.first = m_playAnims.size();
		pushData.second = m_animData["MoveLeft"].number;
		playAnims.push_back(pushData);
		//MV1SetAttachAnimBlendRate(m_modelHandle, pushData.first, moveDir.x);
	}
	if (moveDir.z > 0.0f)
	{
		std::pair<int, int> pushData;
		pushData.first = m_playAnims.size();
		pushData.second = m_animData["MoveFront"].number;
		playAnims.push_back(pushData);
		//MV1SetAttachAnimBlendRate(m_modelHandle, pushData.first, moveDir.x);
	}
	else if (moveDir.z < 0.0f)
	{
		std::pair<int, int> pushData;
		pushData.first = m_playAnims.size();
		pushData.second = m_animData["MoveBack"].number;
		playAnims.push_back(pushData);
		//MV1SetAttachAnimBlendRate(m_modelHandle, pushData.first, moveDir.x);
	}
	if (moveDir.sqLength() == 0)
	{
		std::pair<int, int> pushData;
		pushData.first = m_playAnims.size();
		pushData.second = m_animData["Idle"].number;
		playAnims.push_back(pushData);
	}
	//�f�^�b�`���邩�ǂ�������
	for (auto item : m_playAnims)
	{
		bool detachFlag = true;
		for (auto item2 : playAnims)
		{
			//�������̂����݂�����f�^�b�`���Ȃ�
			if (item.second == item2.second)
			{
				detachFlag = false;
			}
		}
		if (detachFlag)
		{
			MV1DetachAnim(m_modelHandle, item.first);
			printfDx("��������");
		}
	}

	//�A�^�b�`���邩�ǂ�������
	for (auto item : playAnims)
	{
		bool attachFlag = true;
		for (auto item2 : m_playAnims)
		{
			//�������̂����݂�����A�^�b�`���Ȃ�
			if (item.second == item2.second)
			{
				attachFlag = false;
			}
		}
		if (attachFlag)
		{
			MV1AttachAnim(m_modelHandle, item.second);
			printfDx("�o�^������");
		}
	}

	m_playAnims = playAnims;

	//�A�j���[�V������������������
	if (m_playAnims.size() > 1)
	{
		MV1SetAttachAnimBlendRate(m_modelHandle, 0, moveDir.x);
		MV1SetAttachAnimBlendRate(m_modelHandle, 1, moveDir.z);
		printfDx("���������");
	}


	//�A�j���[�V�����̏I���t���[�����擾����
	m_totalAnimTime = m_animData["MoveFront"].endFrame;
	//�A�j���[�V�������J��Ԃ��t���[�����擾����
	m_animLoopTime = m_animData["MoveFront"].loopFrame;
	//�ړ��̍Đ����x�͂��ׂē����Ȃ̂łƂ肠����MoveFront���玝���Ă���
	m_animPlaySpeed = m_animData["MoveFront"].playSpeed;

}


void CharacterBase::SetSpecialAttack(std::string id)
{
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[id].actionTime;
	//�Z�̏���C�͕��A���݂̋C�͂����炷
	m_nowMp -= m_attackData[id].cost;
	//�U���̃^�[�Q�b�g���W�����b�N����
	m_attackTarget = m_targetPos;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;
	//�o���Ă���U���̎�ނ��擾����
	m_attackId = id;
	//�G���߂��ɂ��邩�ǂ�����false�ɂ���
	m_isNearTarget = false;
}

void CharacterBase::SetNormalAttack(bool isPhysical, int time)
{
	//���o���Ă���U���̊m�F���ďo���U����ݒ肷��


	//�i���U���̏ꍇ
	if (isPhysical)
	{
		if (time < kPhysicalAttackConnectTime)
		{

			//�i���U���̈�i�ڂ��o���Ă�����
			if (m_attackId == CommandId::kPhysicalAttack1)
			{
				//�i���U���̓�i�ڂ��o��
				m_attackId = CommandId::kPhysicalAttack2;
			}
			//�i���U���̓�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kPhysicalAttack2)
			{
				//�i���U���̎O�i�ڂ��o��
				m_attackId = CommandId::kPhysicalAttack3;
			}
			//�i���Z�̈�i�ځA��i�ڂ��o���Ă��Ȃ����
			else
			{
				m_attackId = CommandId::kPhysicalAttack1;
			}
		}
		//�i���U�����R���{�ɂȂ�^�C�~���O�łȂ����
		else
		{
			//�i���U���̈�i�ڂ��o��
			m_attackId = CommandId::kPhysicalAttack1;
		}
		//�G���߂��ɂ��邩�ǂ�����false�ɂ���
		m_isNearTarget = false;
	}
	//�C�e�U���̏ꍇ
	else
	{
		if (time < kEnergyAttackConnectTime)
		{

			//�C�e�U���̈�i�ڂ��o���Ă�����
			if (m_attackId == CommandId::kEnergyAttack1)
			{
				//�C�e�U���̓�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack2;
			}
			//�C�e�U���̓�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kEnergyAttack2)
			{
				//�C�e�U���̎O�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack3;
			}
			//�C�e�U���̎O�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kEnergyAttack3)
			{
				//�C�e�U���̎l�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack4;
			}
			//�C�e�U���̈�A��A�O�i�ڂłȂ����
			else
			{
				//�C�e�U���̈�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack1;
			}
		}
		//�C�e�U�����R���{�ɂȂ�^�C�~���O�o�Ȃ����
		else
		{
			//�C�e�U����1�i�ڂ��o��
			m_attackId = CommandId::kEnergyAttack1;
		}
	}
	//�U���̃^�[�Q�b�g���W�����b�N����
	m_attackTarget = m_targetPos;
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[m_attackId].actionTime;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;

}

