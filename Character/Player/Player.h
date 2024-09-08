#pragma once
#include "CharacterBase.h"
#include "Input.h"
#include "PlayerStateBase.h"
class SceneGame;
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input);
	void Draw();

	void SetGameScene(std::shared_ptr<SceneGame> scene) { m_pScene = scene; }

	float GetRota() { return m_rota; }

	MyEngine::Vector3 GetVelo() { return m_rigidbody.GetVelo(); }

	void SetRota(float rota) { m_rota = rota; }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
	
	MyEngine::Vector3 GetTargetPos() { return m_targetPos; }

	DataManager::AttackInfo GetAttackData(std::string id) { return m_attackData[id]; }
	
	bool GetIsOpenSpecialPallet() { return m_isOpenSpecialPallet; }

	PlayerStateBase::PlayerStateKind GetStateKind() { return m_pState->GetKind(); }

	void AddMp(float addPoint) { if(m_nowMp < m_status.mp)m_nowMp += addPoint; }

	/// <summary>
	/// �U���̎�ނ��擾����
	/// </summary>
	/// <param name="attackId">���ׂ����U����ID</param>
	/// <returns>true:Energy,false:Physical</returns>
	bool GetAttackKind(std::string attackId);

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//�Z�b�g���Ă���K�E�Z��Id��Ԃ�
	std::map<std::string, std::string> GetSetSpecialAttackId() { return m_setSpecialAttackId; };
	//�Z�b�g���Ă���K�E�Z�̖��O��Ԃ�
	std::map<std::string, std::string> GetSetSpecialAttackName();
	//�U�����쐬����
	std::shared_ptr<AttackBase> CreateAttack(std::string id);
	//�v���C����G�t�F�N�g��ݒ肷��
	void SetPlayEffect(std::pair<int, int> playHandleData);
	//�v���C���Ă���G�t�F�N�g���X�g�b�v����
	void StopEffect();
	//����p���L���邩�ǂ������擾����
	bool GetIsUpFov() { return m_isUpFov; }
	//����p���L����
	void SetUpFov(bool up) { m_isUpFov = up; }

private:

	float m_rota;

	bool m_isUpFov;

	//�Ō�ɍU�����o���Ă���̎��Ԃ��J�E���g����
	int m_lastAttackTime;
	//�O�̃t���[���̓��͂�ۑ����Ă���
	MyEngine::Vector3 m_lastInput;
	//�K�E�Z�p���b�g���J���Ă��邩�ǂ���
	bool m_isOpenSpecialPallet;

	void ChangeState(std::shared_ptr<PlayerStateBase> state);

	void SetSpecialAttack();

	//�ݒ肵�Ă���Z��Id
	std::map<std::string, std::string> m_setSpecialAttackId;

	//State�p�^�[��
	std::shared_ptr<PlayerStateBase> m_pState;
};

