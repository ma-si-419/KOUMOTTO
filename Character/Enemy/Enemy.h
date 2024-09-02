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

	void SetAiData(std::map<std::string, std::map<Game::AiInfoSort, int>> data) { m_aiStateData = data; }

	std::map<std::string, std::map<Game::AiInfoSort, int>> GetAiData() { return m_aiStateData; }

	//�X�^���Q�[�W�����S�̂̉������邩��Ԃ�
	float GetStanPointRate();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	//State�̍X�V
	void StateUpdate(PlayerStateBase::PlayerStateKind playerState);
private:
	MyEngine::Vector3 debug;

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
	std::map<std::string, std::map<Game::AiInfoSort, int>> m_aiStateData;
};
