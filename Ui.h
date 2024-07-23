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
		//画像ハンドル
		int handle;
		//表示座標
		MyEngine::Vector2 drawPos;
		//画像の横幅
		int width;
		//画像の縦幅
		int height;
	};
	//表示しているUiのデータ
	std::map<std::string, UiStatus> m_showUi;
};

