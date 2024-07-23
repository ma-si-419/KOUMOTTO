#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include <string>
#include "DataManager.h"

class Ui
{
public:
	Ui();
	virtual ~Ui();

	void Init();
	void DrawHpBar(float maxPlayerHp, float playerHp, float maxTargetHp, float targetHp);

	void SetUiPos(std::string name,MyEngine::Vector2 pos);

	void LoadSceneHandle(std::vector<DataManager::UiInfo> data);

private:
	struct UiStatus
	{
		//�摜�n���h��
		int handle;
		//�\�����W
		MyEngine::Vector2 drawPos;
		//�摜�̉���
		int width;
		//�摜�̏c��
		int height;
	};
	//�\�����Ă���Ui�̃f�[�^
	std::map<std::string, UiStatus> m_showUi;
};

