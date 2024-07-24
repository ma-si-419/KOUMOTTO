#include "DataManager.h"
#include "LoadCsv.h"
#include <cassert>

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
	m_LoadAttackData = m_pLoadCsv->LoadFile("data/attackData.csv");

	for (auto& item : m_LoadAttackData)
	{
		//入れるデータ
		Game::AttackInfo pushData;


		//入れるデータに情報を入れる
		pushData.cost = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kCost)]);
		pushData.damageRate = std::stof(item[static_cast<int>(Game::AttackInfoSort::kDamageRate)]);
		pushData.speed = std::stof(item[static_cast<int>(Game::AttackInfoSort::kSpeed)]);
		pushData.radius = std::stof(item[static_cast<int>(Game::AttackInfoSort::kRadius)]);
		pushData.lifeTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kLifeTime)]);
		pushData.attackNum = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kAttackNum)]);
		pushData.isLaser = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kLaser)]));
		pushData.isScatter = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kScatter)]));
		pushData.isTrack = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kTrack)]));
		pushData.attackTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kAttackTime)]);
		pushData.actionTime = std::stoi(item[static_cast<int>(Game::AttackInfoSort::kActionTime)]);
		pushData.isEnergy = static_cast<bool>(std::stoi(item[static_cast<int>(Game::AttackInfoSort::kEnergy)]));
		
		//データを入れる
		m_attackData[item[static_cast<int>(Game::AttackInfoSort::kId)]] = pushData;
	}
}

void DataManager::LoadUiFile()
{

	m_LoadUiData = m_pLoadCsv->LoadFile("data/uiData.csv");
	for (auto& item : m_LoadUiData)
	{
		UiInfo pushData;

		pushData.path = item[static_cast<int>(Game::UiInfoSort::kPath)];
		pushData.posX = stof(item[static_cast<int>(Game::UiInfoSort::kPosX)]);
		pushData.posY = stof(item[static_cast<int>(Game::UiInfoSort::kPosY)]);
		pushData.graphWidth = stoi(item[static_cast<int>(Game::UiInfoSort::kWidth)]);
		pushData.graphHeight = stoi(item[static_cast<int>(Game::UiInfoSort::kHeight)]);

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
