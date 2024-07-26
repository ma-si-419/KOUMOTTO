#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include <string>
#include "DataManager.h"

class Ui
{
public:
	Ui();
	virtual ~Ui();

	void Init();
	void DrawHpBar(float maxPlayerHp, float playerHp, float maxTargetHp, float targetHp);

	void SetUiPos(std::string name,MyEngine::Vector2 pos);

	void LoadSceneHandle(std::vector<DataManager::UiInfo> data);

private:
	struct UiStatus
	{
		//画像ハンドル
		int handle = -1;
		//表示座標
		MyEngine::Vector2 drawPos;
	};
	//前のフレーム表示していたHpバーの状態を残しておく
	int m_lastPlayerHpBarEndPosX;
	int m_lastEnemyHpBarEndPosX;

	//前のフレームのHP
	float m_lastPlayerHp;
	float m_lastEnemyHp;

	//HPがなくなっていくまでの時間カウント
	int m_playerLostHpBarLifeTime;
	int m_enemyLostHpBarLifeTime;
	//表示しているUiのデータ
	std::map<std::string, UiStatus> m_showUi;
};

