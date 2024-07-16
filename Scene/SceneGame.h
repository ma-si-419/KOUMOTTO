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
	//初期化処理
	void Init();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
	//攻撃をシーンに追加
	void AddAttack(std::shared_ptr<AttackBase> pAttack);

private:
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Enemy> m_pEnemy;
	std::vector<std::shared_ptr<AttackBase>> m_pAttack;
};

