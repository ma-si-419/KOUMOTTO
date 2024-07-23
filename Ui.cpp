#include "Ui.h"
#include "DxLib.h"
#include "Game.h"


Ui::Ui()
{
}

Ui::~Ui()
{
}

void Ui::Init()
{
}

void Ui::DrawHpBar(float maxPlayerHp, float playerHp, float maxTargetHp, float targetHp)
{
	DrawGraph(m_showUi["PlayerHpBar"].drawPos.x, m_showUi["PlayerHpBar"].drawPos.y, m_showUi["PlayerHpBar"].handle, true);
	DrawGraph(m_showUi["EnemyHpBar"].drawPos.x, m_showUi["EnemyHpBar"].drawPos.y, m_showUi["EnemyHpBar"].handle, true);

	//HPバーの開始位置
	MyEngine::Vector2 playerHpBarStartPos;

	playerHpBarStartPos.x = m_showUi["PlayerHpBar"].drawPos.x - m_showUi["PlayerHpBar"].width / 2;
	playerHpBarStartPos.y = m_showUi["PlayerHpBar"].drawPos.y - m_showUi["PlayerHpBar"].height / 2;
	//HPバーの終了位置
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi["PlayerHpBar"].drawPos.x + m_showUi["PlayerHpBar"].width / 2;
	playerHpBarEndPos.y = m_showUi["PlayerHpBar"].drawPos.y + m_showUi["PlayerHpBar"].height / 2;

	playerHpBarEndPos.x = playerHpBarEndPos.x * (playerHp / maxPlayerHp);
	
	DrawBox(playerHpBarStartPos.x, playerHpBarStartPos.y, playerHpBarEndPos.x, playerHpBarEndPos.y,
		GetColor(255, 255, 255), true);
	
	//HPバーの開始位置
	MyEngine::Vector2 enemyHpBarStartPos;

	enemyHpBarStartPos.x = m_showUi["EnemyHpBar"].drawPos.x - m_showUi["EnemyHpBar"].width / 2;
	enemyHpBarStartPos.y = m_showUi["EnemyHpBar"].drawPos.y - m_showUi["EnemyHpBar"].height / 2;
	//HPバーの終了位置
	MyEngine::Vector2 enemyHpBarEndPos;

	enemyHpBarEndPos.x = m_showUi["EnemyHpBar"].drawPos.x + m_showUi["EnemyHpBar"].width / 2;
	enemyHpBarEndPos.y = m_showUi["EnemyHpBar"].drawPos.y + m_showUi["EnemyHpBar"].height / 2;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	enemyHpBarEndPos.x = enemyHpBarStartPos.x + enemyHpBarLength * (targetHp / maxTargetHp);

	DrawBox(enemyHpBarStartPos.x, enemyHpBarStartPos.y, enemyHpBarEndPos.x, enemyHpBarEndPos.y,
		GetColor(255, 0, 0), true);
}


void Ui::SetUiPos(std::string name, MyEngine::Vector2 pos)
{
	m_showUi[name].drawPos = pos;
}

void Ui::LoadSceneHandle(std::vector<DataManager::UiInfo> data)
{
	//中のものをすべて消す
	m_showUi.clear();

	for (auto& item : data)
	{
		//ゲームシーンのUIをロードする
		UiStatus pushData;
		//データを入れる
		pushData.drawPos = MyEngine::Vector2(item.posX, item.posY);
		pushData.width = item.graphWidth;
		pushData.height = item.graphHeight;
		//画像のロード
		pushData.handle = LoadGraph(("data/image/" + item.path + ".png").c_str());
		//画像の名前でマップに登録
		m_showUi[item.path] = pushData;
	}
}
