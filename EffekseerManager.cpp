#include "EffekseerManager.h"
#include "LoadCsv.h"
#include <string>
#include "EffekseerForDXLib.h"

EffekseerManager::~EffekseerManager()
{
	for (auto& item : m_pEffekseerDatas)
	{
		item->Final();
	}
	m_pEffekseerDatas.clear();
}

void EffekseerManager::LoadData()
{
	LoadCsv load;

	//�G�t�F�N�g�̃f�[�^�ǂݍ���
	std::vector<std::vector<std::string>> loadEffectData = load.LoadFile("data/csv/effekseerHandleData.csv");

	for (auto& item : loadEffectData)
	{
		std::string dataName = "data/effekseer/" + item[static_cast<int>(Game::EffeckInfoSort::kFileName)] + ".efk";

		//�n���h�������[�h���ă}�b�v�ɓo�^
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].first = LoadEffekseerEffect(dataName.c_str(), stof(item[static_cast<int>(Game::EffeckInfoSort::kMag)]));
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].second = stoi(item[static_cast<int>(Game::EffeckInfoSort::kLoopFrame)]);
	}
}

void EffekseerManager::Update()
{
	std::vector<int> deleteDatas;
	int count = 0;
	//�G�t�F�N�g�̃A�b�v�f�[�g
	for (auto& item : m_pEffekseerDatas)
	{
		item->Update();
		if (item->GetDeleteFlag())
		{
			deleteDatas.push_back(count);
		}
		count++;
	}
	//�폜�t���O�������Ă�����폜����
	for (auto& item : deleteDatas)
	{
		m_pEffekseerDatas.erase(m_pEffekseerDatas.begin() + item);
	}
}

void EffekseerManager::Draw()
{
	for (auto& item : m_pEffekseerDatas)
	{
		item->Draw();
	}
}
