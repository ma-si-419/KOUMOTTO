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
		int attackStartTime = 0;
		int attackEndTime = 0;
		int actionTotalTime = 0;
		bool isEnergy = false;
		int hitEffect = 0;
		std::string effekseerName = "empty";
	};

	DataManager();
	~DataManager();

	void Init();

	void LoadAttackFile();
	void LoadEffekseerHandle();
	void LoadUiFile();
	void LoadAiFile();
	void LoadAnimationFile();

	//攻撃のデータを取得
	std::map<std::string, AttackInfo> GetAttackData() { return m_attackData; };
	//Effekseerのデータを取得
	std::map<std::string, int> GetEffekseerHandle() { return m_effekseerHandle; };

	//ゲームシーンのUIのパス取得
	std::vector<UiInfo> GetUiData(Game::SceneNum sceneNum);
	//エネミーのAI取得
	std::map<std::string, std::vector<int>> GetAiData() { return m_aiData; };
	//アニメーションのデータ取得
	std::vector<std::vector<std::string>> GetAnimationData() { return m_LoadAnimationData; }
private:

	//ファイルから読み込んだデータ保存先
	std::vector<std::vector<std::string>> m_LoadAttackData;
	std::vector<std::vector<std::string>> m_LoadUiData;
	std::vector<std::vector<std::string>> m_LoadAiData;
	std::vector<std::vector<std::string>> m_LoadAnimationData;


	//攻撃のデータ
	std::map<std::string, AttackInfo> m_attackData;
	//Effekseerのロードしたハンドルを保存しておく
	std::map<std::string, int> m_effekseerHandle;

	//画像のデータ
	std::vector<UiInfo> m_sceneGameUiData;
	std::vector<UiInfo> m_sceneTitleUiData;
	std::vector<UiInfo> m_sceneSelectUiData;

	//Aiのデータ
	std::map<std::string, std::vector<int>> m_aiData;

	//ファイルロードできるクラス
	std::shared_ptr<LoadCsv> m_pLoadCsv;
};

