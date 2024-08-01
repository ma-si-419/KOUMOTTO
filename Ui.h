#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include <string>
#include "DataManager.h"
#include <memory>

class Player;
class Enemy;
class Ui
{
public:
	Ui();
	virtual ~Ui();

	void Init();
	void DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);

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

