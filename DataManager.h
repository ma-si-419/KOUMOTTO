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
		std::string path;
		float posX;
		float posY;
		float graphWidth;
		float graphHeight;
	};
	DataManager();
	~DataManager();

	void Init();

	void LoadAttackFile();
	void LoadUiFile();

	//�U���̃f�[�^���擾
	std::map<std::string, Game::AttackInfo> GetAttackData() { return m_attackData; };
	//�Q�[���V�[����UI�̃p�X�擾
	std::vector<UiInfo> GetUiData(Game::SceneNum sceneNum);

private:


	//�t�@�C������ǂݍ��񂾃f�[�^�ۑ���
	std::vector<std::vector<std::string>> m_LoadAttackData;
	std::vector<std::vector<std::string>> m_LoadUiData;

	//�U���̃f�[�^
	std::map<std::string, Game::AttackInfo> m_attackData;

	//�摜�̃f�[�^
	std::vector<UiInfo> m_sceneGameUiData;
	std::vector<UiInfo> m_sceneTitleUiData;
	std::vector<UiInfo> m_sceneSelectUiData;

	//�t�@�C�����[�h�ł���N���X
	std::shared_ptr<LoadCsv> m_pLoadCsv;
};

