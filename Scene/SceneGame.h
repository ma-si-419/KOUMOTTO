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
	//‰Šú‰»ˆ—
	void Init();
	//XVˆ—
	void Update(MyEngine::Input input);
	//•`‰æˆ—
	void Draw();
	//I—¹ˆ—
	void End();
private:
	Physics* m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Enemy> m_pEnemy;
};

