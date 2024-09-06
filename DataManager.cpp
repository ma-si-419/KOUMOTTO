#include "DataManager.h"
#include "LoadCsv.h"
#include <cassert>
#include "EffekseerForDXLib.h"

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
}

void DataManager::LoadAttackFile()
{
	//攻撃のデータ読み込み
	std::vector<std::vector<std::string>> loadAttackData = m_pLoadCsv->LoadFile("data/attackData.csv");

	for (auto& item : loadAttackData)
	{
		//入れるデータ
		DataManager::AttackInfo pushData;


		//入れるデータに情報を入れる
		pushData.name = item[static_cast<int>(Game::AttackInfoSort::kName)];
		pushData.cost = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kCost)]);
		pushData.damageRate = std::stof(item[static_cast<int>(Game::AttackInfoSort::kDamageRate)]);
		pushData.stanDamage = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kStanDamage)]);
		pushData.speed = std::stof(item[static_cast<int>(Game::AttackInfoSort::kSpeed)]);
		pushData.radius = std::stof(item[static_cast<int>(Game::AttackInfoSort::kRadius)]);
		pushData.lifeTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kLifeTime)]);
		pushData.attackNum = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kAttackNum)]);
		pushData.isLaser = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kLaser)]));
		pushData.isScatter = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kScatter)]));
		pushData.isTrack = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kTrack)]));
		pushData.attackStartTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kAttackStartTime)]);
		pushData.attackEndTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kAttackEndTime)]);
		pushData.actionTotalTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kActionTotalTime)]);
		pushData.isEnergy = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kEnergy)]));
		pushData.hitEffect = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kHitEffect)]);
		pushData.isSpecial = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kIsSpecial)]));
		pushData.effekseerName = item[static_cast<int>(Game::AttackInfoSort::kEffekseerName)];

		//データを入れる
		m_attackData[item[static_cast<int>(Game::AttackInfoSort::kId)]] = pushData;
	}
}

void DataManager::LoadEffekseerFile()
{
	//エフェクトのデータ読み込み
	std::vector<std::vector<std::string>> loadEffectData = m_pLoadCsv->LoadFile("data/effekseerHandleData.csv");

	for (auto& item : loadEffectData)
	{
		std::string dataName = "data/effekseer/" + item[static_cast<int>(Game::EffeckInfoSort::kFileName)] + ".efk";

		//ハンドルをロードしてマップに登録
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].first = LoadEffekseerEffect(dataName.c_str(), stoi(item[static_cast<int>(Game::EffeckInfoSort::kMag)]));
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].second = stoi(item[static_cast<int>(Game::EffeckInfoSort::kLoopFrame)]);
	}
}

void DataManager::LoadUiFile()
{
	std::vector<std::vector<std::string>> loadUiData = m_pLoadCsv->LoadFile("data/uiData.csv");
	for (auto& item : loadUiData)
	{
		UiInfo pushData;

		pushData.path = item[static_cast<int>(Game::UiInfoSort::kPath)];
		pushData.posX = stof(item[static_cast<int>(Game::UiInfoSort::kPosX)]);
		pushData.posY = stof(item[static_cast<int>(Game::UiInfoSort::kPosY)]);

		//使うシーンによって入れる配列を変える
		if (item[static_cast<int>(Game::UiInfoSort::kScene)] == "Title")
		{
			m_sceneTitleUiData.push_back(pushData);
		}
		else if (item[static_cast<int>(Game::UiInfoSort::kScene)] == "Select")
		{
			m_sceneSelectUiData.push_back(pushData);
		}
		else if (item[static_cast<int>(Game::UiInfoSort::kScene)] == "Game")
		{
			m_sceneGameUiData.push_back(pushData);
		}
		else
		{
			//どれにも当てはまらなかったらエラー
			assert(false);
		}
	}
}

void DataManager::LoadAiFile()
{
	std::vector<std::vector<std::string>> loadAiData = m_pLoadCsv->LoadFile("data/enemyAiData.csv");
	//入れるデータ
	std::map<std::string, std::vector<int>> pushData;
	for (auto& item : loadAiData)
	{
		std::vector<int> inputData;

		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kIdle)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kMove)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kDash)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kDodge)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kAttack)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kGuard)]));
		inputData.push_back(stoi(item[static_cast<int>(Game::AiInfoSort::kCharge)]));
		pushData[item[static_cast<int>(Game::AiInfoSort::kPlayerState)]] = inputData;
	}
	m_aiData = pushData;
}

void DataManager::LoadAnimationFile()
{
	m_LoadAnimationData = m_pLoadCsv->LoadFile("data/animationData.csv");
}

std::vector<DataManager::UiInfo> DataManager::GetUiData(Game::SceneNum sceneNum)
{
	if (sceneNum == Game::SceneNum::kTitle)
	{
		return m_sceneTitleUiData;
	}
	else if (sceneNum == Game::SceneNum::kSelect)
	{
		return m_sceneSelectUiData;
	}
	else if (sceneNum == Game::SceneNum::kGame)
	{
		return m_sceneGameUiData;
	}
	else
	{
		//どれにも当てはまらなかったらエラー
		assert(false);
	}
	std::vector<DataManager::UiInfo> empty;
	return empty;
}