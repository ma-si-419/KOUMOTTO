#pragma once
#include "Game.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class LoadCsv;
class DataManager
{
public:
	struct UiInfo
	{
		std::string path = "empty";
		float posX = 0;
		float posY = 0;
	};
	struct AttackInfo
	{
		std::string name = "empty";
		int cost = 0;
		float damageRate = 0;
		int stanDamage = 0;
		int damage = 0;
		float speed = 0;
		float radius = 0;
		int lifeTime = 0;
		int attackNum = 0;
		bool isLaser = false;
		bool isScatter = false;
		bool isTrack = false;
		int attackTime = 0;
		int actionTime = 0;
		bool isEnergy = false;
		int hitEffect = 0;
	};

	DataManager();
	~DataManager();

	void Init();

	void LoadAttackFile();
	void LoadUiFile();

	//攻撃のデータを取得
	std::map<std::string, AttackInfo> GetAttackData() { return m_attackData; };
	//ゲームシーンのUIのパス取得
	std::vector<UiInfo> GetUiData(Game::SceneNum sceneNum);

private:


	//ファイルから読み込んだデータ保存先
	std::vector<std::vector<std::string>> m_LoadAttackData;
	std::vector<std::vector<std::string>> m_LoadUiData;

	//攻撃のデータ
	std::map<std::string, AttackInfo> m_attackData;

	//画像のデータ
	std::vector<UiInfo> m_sceneGameUiData;
	std::vector<UiInfo> m_sceneTitleUiData;
	std::vector<UiInfo> m_sceneSelectUiData;

	//ファイルロードできるクラス
	std::shared_ptr<LoadCsv> m_pLoadCsv;
};

