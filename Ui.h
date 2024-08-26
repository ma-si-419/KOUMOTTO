#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include "DataManager.h"
#include <memory>

class Player;
class Enemy;
class Ui
{
public:
	enum class GameOverItem
	{
		kRetry,
		kEnd,
		kItemNum
	};
public:
	Ui();
	virtual ~Ui();

	void Init();
	void DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);

	void SetUiPos(std::string name, MyEngine::Vector2 pos);

	void LoadUiHandle(std::vector<DataManager::UiInfo> data);

	void AddShowDamage(MyEngine::Vector3 pos, int damage);

	void DrawDamage();

	void DrawGameOver(int arrowPos);
private:


	struct UiStatus
	{
		//�摜�n���h��
		int handle = -1;
		//�\�����W
		MyEngine::Vector2 drawPos;
	};
	//�G�l�~�[�ɗ^�����_���[�W��\������Ƃ��ɕK�v�ȍ\����
	struct DamageShowStatus
	{
		//�\������_���[�W
		std::string damage = "empty";
		//�\������
		float time = 0;
		//�\�����W
		MyEngine::Vector2 pos;
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

	//�_���[�W��\������Ƃ��̃t�H���g�n���h��
	int m_damageFontHandle;
	//�Q�[���I�[�o�[�̎��̑I�������̃t�H���g�n���h��
	int m_gameOverFontHandle;
	//�\������_���[�W�̔z��
	std::vector<DamageShowStatus> m_showDamage;
	//�I�����Ă�����𓮂����ۂɎg�p����ϐ�
	float m_shakeArrowNum;

	//�\�����Ă���Ui�̃f�[�^
	std::map<std::string, UiStatus> m_showUi;
};