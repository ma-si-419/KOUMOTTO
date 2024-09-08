#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"
#include <memory>
class EnemyStateBase;
class SceneGame;
class Ui;
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene);
	void Draw();

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	void SetUi(std::shared_ptr<Ui> pUi);

	void InitAiState(std::shared_ptr<SceneGame> scene);

	void SetAiData(std::map<std::string, std::vector<int>> data) { m_aiStateData = data; }

	std::map<std::string, std::vector<int>> GetAiData() { return m_aiStateData; }

	//�X�^���Q�[�W�����S�̂̉������邩��Ԃ�
	float GetStanPointRate();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	MyEngine::Vector3 GetTargetPos() { return m_targetPos; }

	//�U���̏���Ԃ�
	std::map<std::string, DataManager::AttackInfo> GetAttackData() { return m_attackData; }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	//State�̍X�V
	void StateUpdate(std::shared_ptr<Player> player);

	//�U�����쐬����
	std::shared_ptr<AttackBase> CreateAttack(std::string id);

	//�v���C����G�t�F�N�g��ݒ肷��
	void SetPlayEffect(std::pair<int, int> playHandleData);
	//�v���C���Ă���G�t�F�N�g���X�g�b�v����
	void StopEffect();

	//�����ʒu�ɖ߂�
	void InitPos();
private:
	//�X�^���Q�[�W
	float m_stanPoint;
	//�Ō�ɍU�����󂯂Ă��痧��������
	int m_lastHitDamageTime;
	//�Ԋu���J�����ɍU�����󂯂���
	int m_comboCount;
	//�󂯂��_���[�W��\�����邽�߂�Ui�Ǘ��N���X�̎Q�Ƃ�����
	std::shared_ptr<Ui> m_pUi;
	//State�p�^�[��
	std::shared_ptr<EnemyStateBase> m_pState;
	//�G��State�ɂ���ē����̊m����ω�������
	std::map<std::string, std::vector<int>> m_aiStateData;
	//�G�l�~�[���g�p����G�t�F�N�g�n���h�����������Ă���
	std::map<std::string, int> m_effectHandles;
};
