#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include "DataManager.h"
#include <memory>

class Player;
class Enemy;
class Ui
{
public:
	Ui();
	virtual ~Ui();

	void Init();
	void DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);

	void SetUiPos(std::string name,MyEngine::Vector2 pos);

	void LoadUiHandle(std::vector<DataManager::UiInfo> data);

	void AddShowDamage(MyEngine::Vector3 pos,int damage);
		
	void DrawDamage();
private:
	struct UiStatus
	{
		//画像ハンドル
		int handle = -1;
		//表示座標
		MyEngine::Vector2 drawPos;
	};
	//エネミーに与えたダメージを表示するときに必要な構造体
	struct DamageShowStatus
	{
		//表示するダメージ
		int damage = 0;
		//表示時間
		float time = 0;
		//表示座標
		MyEngine::Vector2 pos;
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

	//フォントハンドル
	int m_fontHandle;
	//表示するダメージの配列
	std::vector<DamageShowStatus> m_showDamage;

	//表示しているUiのデータ
	std::map<std::string, UiStatus> m_showUi;
};

