#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
class Physics;
class Player;
class GameCamera;
class Enemy;
class AttackBase;
class Ui;
class SceneGame : public SceneBase , public std::enable_shared_from_this<SceneGame>
{
public:
	SceneGame(SceneManager& sceneManager,DataManager& dataManager);
	~SceneGame();
	//����������
	void Init();
	//���g���C���̏���������
	void RetryInit();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�U�����V�[���ɒǉ�
	void AddAttack(std::shared_ptr<AttackBase> pAttack);
	//�Q�[���I�[�o�[���ɌĂяo��
	void GameOver() { m_isGameOver = true; }

private:
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<GameCamera> m_pGameCamera;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Ui> m_pUi;
	std::vector<std::shared_ptr<AttackBase>> m_pAttacks;
	int handle;
	//�Q�[���I�[�o�[���ǂ���
	bool m_isGameOver;
	//�Q�[���I�[�o�[���̑I�����Ă���A�C�e��
	int m_gameOverSelectItemNum;
	//�o�g�����J�n���Ă��邩�ǂ���
	bool m_isStartBattle;
	//�ҋ@���Ԃ��v��
	int m_standByTime;
};

