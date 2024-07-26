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
		int handle = -1;
		//�\�����W
		MyEngine::Vector2 drawPos;
	};
	//�O�̃t���[���\�����Ă���Hp�o�[�̏�Ԃ��c���Ă���
	int m_lastPlayerHpBarEndPosX;
	int m_lastEnemyHpBarEndPosX;

	//�O�̃t���[����HP
	float m_lastPlayerHp;
	float m_lastEnemyHp;

	//HP���Ȃ��Ȃ��Ă����܂ł̎��ԃJ�E���g
	int m_playerLostHpBarLifeTime;
	int m_enemyLostHpBarLifeTime;
	//�\�����Ă���Ui�̃f�[�^
	std::map<std::string, UiStatus> m_showUi;
};

