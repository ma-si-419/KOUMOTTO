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
	SceneGame(SceneManager& sceneManager,DataManager& dataManager, SoundManager& soundManager);
	~SceneGame();
	//初期化処理
	void Init();
	//リトライ時の初期化処理
	void RetryInit();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
	//攻撃をシーンに追加
	void AddAttack(std::shared_ptr<AttackBase> pAttack);
	//ゲームオーバー時に呼び出す
	void GameOver() { m_isGameOver = true; }
	//効果音を再生する
	void PlaySE(std::string name,int playType);
	//効果音を止める
	void StopSE(std::string name) { m_soundManager.Stop(name); }
	
	//サウンドのハンドルを取得する
	int GetSEHandle(std::string name) { return m_soundManager.GetHandle(name); }

private:
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<GameCamera> m_pGameCamera;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Ui> m_pUi;
	std::vector<std::shared_ptr<AttackBase>> m_pAttacks;

	
	//ゲームオーバーかどうか
	bool m_isGameOver;
	//ゲームクリアかどうか
	bool m_isGameClear;
	//ゲームオーバー時の選択しているアイテム
	int m_gameOverSelectItemNum;
	//バトルが開始しているかどうか
	bool m_isStartBattle;
	//待機時間を計測
	int m_standByTime;
	//クリア時の画像がおいてある座標
	int m_clearGraphPosX;
	//バトル終盤かどうか
	bool m_isFinalStage;
};

