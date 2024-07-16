#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
class Physics;
class Player;
class Camera;
class Enemy;
class AttackBase;
class SceneGame : public SceneBase , public std::enable_shared_from_this<SceneGame>
{
public:
	SceneGame(SceneManager& sceneManager);
	~SceneGame();
	//����������
	void Init();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�U�����V�[���ɒǉ�
	void AddAttack(std::shared_ptr<AttackBase> pAttack);

private:
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Enemy> m_pEnemy;
	std::vector<std::shared_ptr<AttackBase>> m_pAttack;
};

