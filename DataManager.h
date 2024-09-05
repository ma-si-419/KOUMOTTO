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
		bool isSpecial = false;
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

	//�U���̃f�[�^���擾
	std::map<std::string, AttackInfo> GetAttackData() { return m_attackData; };
	//Effekseer�̃f�[�^���擾
	std::map<std::string, int> GetEffekseerHandle() { return m_effekseerHandle; };

	//�Q�[���V�[����UI�̃p�X�擾
	std::vector<UiInfo> GetUiData(Game::SceneNum sceneNum);
	//�G�l�~�[��AI�擾
	std::map<std::string, std::vector<int>> GetAiData() { return m_aiData; };
	//�A�j���[�V�����̃f�[�^�擾
	std::vector<std::vector<std::string>> GetAnimationData() { return m_LoadAnimationData; }
private:

	//�t�@�C������ǂݍ��񂾃f�[�^�ۑ���
	std::vector<std::vector<std::string>> m_LoadAttackData;
	std::vector<std::vector<std::string>> m_LoadUiData;
	std::vector<std::vector<std::string>> m_LoadAiData;
	std::vector<std::vector<std::string>> m_LoadAnimationData;


	//�U���̃f�[�^
	std::map<std::string, AttackInfo> m_attackData;
	//Effekseer�̃��[�h�����n���h����ۑ����Ă���
	std::map<std::string, int> m_effekseerHandle;

	//�摜�̃f�[�^
	std::vector<UiInfo> m_sceneGameUiData;
	std::vector<UiInfo> m_sceneTitleUiData;
	std::vector<UiInfo> m_sceneSelectUiData;

	//Ai�̃f�[�^
	std::map<std::string, std::vector<int>> m_aiData;

	//�t�@�C�����[�h�ł���N���X
	std::shared_ptr<LoadCsv> m_pLoadCsv;
};

