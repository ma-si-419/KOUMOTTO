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
	//�U���̃f�[�^�ǂݍ���
	m_LoadAttackData = m_pLoadCsv->LoadFile("data/attackData.csv");

	for (auto& item : m_LoadAttackData)
	{
		//�����f�[�^
		DataManager::AttackInfo pushData;


		//�����f�[�^�ɏ�������
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

		//�f�[�^������
		m_attackData[item[static_cast<int>(Game::AttackInfoSort::kId)]] = pushData;
	}
}

void DataManager::LoadEffekseerHandle()
{
	for (auto item : m_attackData)
	{
		std::string name = "data/effekseer/" + item.second.effekseerName + ".efk";

		m_effekseerHandle[item.second.effekseerName] = LoadEffekseerEffect(name.c_str());
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

		//�g���V�[���ɂ���ē����z���ς���
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
			//�ǂ�ɂ����Ă͂܂�Ȃ�������G���[
			assert(false);
		}
	}
}

void DataManager::LoadAiFile()
{
	m_LoadAiData = m_pLoadCsv->LoadFile("data/enemyAiData.csv");
	//�����f�[�^
	std::map<std::string, std::vector<int>> pushData;
	for (auto& item : m_LoadAiData)
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
		//�ǂ�ɂ����Ă͂܂�Ȃ�������G���[
		assert(false);
	}
	std::vector<DataManager::UiInfo> empty;
	return empty;
}