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
	//初期化処理
	void Init();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
private:
	Physics* m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
};

