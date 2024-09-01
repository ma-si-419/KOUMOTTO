#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "DataManager.h"
#include "Game.h"
#include <map>
#include <string>

class AttackBase;
class Physics;
class SceneGame;
class CharacterBase : public Collidable
{
public:
	struct Status
	{
		float hp = 30000;
		float mp = 300;
		float atk = 100;
		float def = 200;
	};
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CharacterBase(const TCHAR* graph, ObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(std::shared_ptr<Physics> physics) abstract;

	virtual void RetryInit() abstract;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() abstract;

	//�ő�̗͂�Ԃ�
	Status GetStatus() { return m_status; }
	//���݂̗̑͂�Ԃ�
	float GetNowHp() { return m_nowHp; }
	//���݂̋C�͗ʂ�Ԃ�
	float GetNowMp() { return m_nowMp; }
	//���b�N�I�����Ă���G�̍��W��ݒ肷��
	void SetTargetPos(MyEngine::Vector3 pos);
	//�U���̏���ݒ肷��
	void SetAttackData(std::map<std::string, DataManager::AttackInfo> data) { m_attackData = data; }
	//�����Ȃ����Ԃ�ݒ肷��
	void SetStanTime(int time) { m_stanTime = time; }
	//�U���ɕK�v�ȋC�͗ʂ��擾����
	int GetAttackCost(std::string Id) { return m_attackData[Id].cost; }
	//�A�j���[�V�����̃f�[�^���擾����
	void LoadAnimationData(bool isPlayer);
	//�K�E�Z���o���Ă����Ԃɕω�������
	void PlaySpecialAttack(std::string id);
	//�A�j���[�V�����𑝂₷
	void ChangeAnim(std::string animName);
	//�A�j���[�V�����u�����h
	void MoveAnim(MyEngine::Vector3 moveDir);
protected:
	enum class AnimationInfoSort
	{
		kName,
		kNumber,
		kStartFrame,
		kLoopFrame,
		kEndFrame,
		kPlaySpeed
	};
	struct AnimationInfo
	{
		int number = 0;
		int startFrame = 0;
		int loopFrame = 0;
		int endFrame = 0;
		float playSpeed = 0.0f;
	};
	//���f���n���h��
	int m_modelHandle;
	//�o�^����Physics�������Ă���
	std::shared_ptr<Physics> m_pPhysics;
	//�V�[���̃|�C���^�������Ă���
	std::shared_ptr<SceneGame> m_pScene;

	//�U���̃f�[�^
	std::map<std::string, DataManager::AttackInfo> m_attackData;
	//�A�j���[�V�����̃f�[�^
	std::map<std::string, AnimationInfo> m_animData;
	//��{�I�ȃX�e�[�^�X
	Status m_status;
	//���݂̗̑�
	float m_nowHp;
	//���݂̋C��
	float m_nowMp;
	//�����Ȃ�����
	int m_stanTime;
	//�U�����o���Ă��邩�ǂ���
	bool m_isAttack;
	//�^�[�Q�b�g���߂��ɂ��邩�ǂ���
	bool m_isNearTarget;
	//���Đ����Ă���A�j���[�V����
	std::vector<std::pair<int, int>> m_playAnims;
	//�A�j���[�V�����̍Đ����x
	float m_animPlaySpeed;
	//�A�j���[�V�����̑��Đ�����
	float m_totalAnimTime;
	//�A�j���[�V�����̍Đ�����
	float m_animTime;
	//�A�j���[�V�����̃��[�v����t���[��
	float m_animLoopTime;

	//�o���Ă���U��
	std::string m_attackId;
	//�U�����o�����^�C�~���O�̓G�̍��W
	MyEngine::Vector3 m_attackTarget;
	//���b�N�I�����Ă��鑊��̍��W
	MyEngine::Vector3 m_targetPos;
	/// <summary>
	/// �U�����쐬����
	/// </summary>
	/// <param name="id">�U����ID</param>
	/// <returns>�U���̃|�C���^</returns>
	std::shared_ptr<AttackBase> CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer);

	


	//�ʏ�U�����o���Ă��Ԃɕω�������
	void SetNormalAttack(bool isPhysical, int time);
};