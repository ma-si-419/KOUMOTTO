#pragma once
#include "SceneBase.h"
#include <memory>
class Physics;
class Player;
class Camera;
class Enemy;
class SceneGame : public SceneBase
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
private:
	Physics* m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Enemy> m_pEnemy;
};

