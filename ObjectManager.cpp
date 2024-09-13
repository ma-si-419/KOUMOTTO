#include "ObjectManager.h"
#include "ObjectBase.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::LoadData(Game::SceneNum scene)
{
	LoadCsv loadCsv;

	std::vector<std::vector<std::string>> data = loadCsv.LoadFile("data/csv/objectData.csv");

	for (auto item : data)
	{
		ObjectData pushData;

		pushData.path = item[static_cast<int>(ObjectDataInfo::kPath)];
		pushData.pos.x = std::stoi(item[static_cast<int>(ObjectDataInfo::kPosX)]);
		pushData.pos.y = std::stoi(item[static_cast<int>(ObjectDataInfo::kPosY)]);
		pushData.pos.z = std::stoi(item[static_cast<int>(ObjectDataInfo::kPosZ)]);
		pushData.rota.x = std::stoi(item[static_cast<int>(ObjectDataInfo::kRotaX)]);
		pushData.rota.y = std::stoi(item[static_cast<int>(ObjectDataInfo::kRotaY)]);
		pushData.rota.z = std::stoi(item[static_cast<int>(ObjectDataInfo::kRotaZ)]);
		pushData.scale.x = std::stoi(item[static_cast<int>(ObjectDataInfo::kScaleX)]);
		pushData.scale.x = std::stoi(item[static_cast<int>(ObjectDataInfo::kScaleY)]);
		pushData.scale.x = std::stoi(item[static_cast<int>(ObjectDataInfo::kScaleZ)]);
		
	}
	
}

void ObjectManager::SetObject()
{
}

void ObjectManager::Draw()
{
	for (auto& item : m_pObject)
	{
		item->Draw();
	}
}
