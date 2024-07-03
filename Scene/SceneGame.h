#pragma once
#include "SceneBase.h"
#include <memory>
class Physics;
class Player;
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
};

