#include "Ui.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//Hpバーの大きさ
	constexpr int kHpBarHeight = 15;
	constexpr int kHpBarWidth = 188;

	//体力が減った時の赤いゲージが減少していくまでの時間
	constexpr int kLostHpBarLifeTime = 30;
	//赤いゲージが減少する速度
	constexpr int kLostHpBarDecreaseSpeed = 3;
	//ダメージを受けた時に揺れる大きさ
	constexpr int kShakeScale = 15;
	//ダメージを受けた時に揺れる大きさの半分
	constexpr int kShakeHalfScale = static_cast<int>(kShakeScale * 0.5);
	//HPバーの座標(画像の座標に対してのHPバーの座標のずれを直す)
	constexpr int kHpBarPosX = -52;
	constexpr int kHpBarPosY = 8;
}

Ui::Ui() :
	m_lastPlayerHp(0),
	m_lastEnemyHp(0),
	m_lastPlayerHpBarEndPosX(0),
	m_lastEnemyHpBarEndPosX(0),
	m_playerLostHpBarLifeTime(0),
	m_enemyLostHpBarLifeTime(0),
	m_showUi()
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
	//ダメージを受けた際プレイヤーのHPバーを揺らす大きさ
	MyEngine::Vector2 playerHpBarShakeSize(0, 0);
	//ダメージを受けた際エネミーのHPバーを揺らす大きさ
	MyEngine::Vector2 enemyHpBarShakeSize(0, 0);

	//前のフレームよりプレイヤーのHPが減っていたら
	if (m_lastPlayerHp > playerHp)
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_playerLostHpBarLifeTime = kLostHpBarLifeTime;

		//プレイヤーのHPバーの座標を揺らす
		playerHpBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}
	//前のフレームよりエネミーのHPが減っていたら
	if (m_lastEnemyHp > targetHp)
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_enemyLostHpBarLifeTime = kLostHpBarLifeTime;
		//エネミーのHPバーの座標を揺らす
		enemyHpBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}

	//体力量を覚えておく
	m_lastPlayerHp = playerHp;
	m_lastEnemyHp = targetHp;

	//画像の名前
	std::string playerHpBar = "PlayerHpBar";
	std::string enemyHpBar = "EnemyHpBar";

	//プレイヤーHPバーの開始位置
	MyEngine::Vector2 playerHpBarStartPos;
	playerHpBarStartPos.x = m_showUi[playerHpBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	playerHpBarStartPos.y = m_showUi[playerHpBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//プレイヤーHPバーの終了位置
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi[playerHpBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	playerHpBarEndPos.y = m_showUi[playerHpBar].drawPos.y - kHpBarHeight + kHpBarPosY;


	float playerHpBarLength = playerHpBarEndPos.x - playerHpBarStartPos.x;


	//エネミーHPバーの開始位置
	MyEngine::Vector2 enemyHpBarStartPos;
	enemyHpBarStartPos.x = m_showUi[enemyHpBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	enemyHpBarStartPos.y = m_showUi[enemyHpBar].drawPos.y + kHpBarHeight + kHpBarPosY;
	
	//エネミーHPバーの終了位置
	MyEngine::Vector2 enemyHpBarEndPos;
	
	enemyHpBarEndPos.x = m_showUi[enemyHpBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	enemyHpBarEndPos.y = m_showUi[enemyHpBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	playerHpBarStartPos += playerHpBarShakeSize;
	playerHpBarEndPos += playerHpBarShakeSize;

	enemyHpBarStartPos += enemyHpBarShakeSize;
	enemyHpBarEndPos += enemyHpBarShakeSize;

	//プレイヤーのHPバー表示
	DrawBox(static_cast<int>(playerHpBarStartPos.x), static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 255, 64), true);
	//エネミーのHPバーの表示
	DrawBox(static_cast<int>(enemyHpBarStartPos.x), static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 255, 64), true);


	//体力に応じて上にかぶせるボックスの長さを変化させる
	int playerLostHpBoxPosX = static_cast<int>(playerHpBarStartPos.x + playerHpBarLength * (playerHp / maxPlayerHp));
	int enemyLostHpBoxPosX = static_cast<int>(enemyHpBarStartPos.x + enemyHpBarLength * (targetHp / maxTargetHp));

	//プレイヤーのHPバーの上にかぶせるボックス表示
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 64, 64), true);
	//プレイヤーのHPバーの上にかぶせるボックス表示
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 64, 64), true);

	//赤いバーの初期化
	if (m_lastPlayerHpBarEndPosX == 0 && m_lastEnemyHpBarEndPosX == 0)
	{
		m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
	}

	//赤いバーが消えるまでの時間をカウントする
	m_playerLostHpBarLifeTime--;
	m_enemyLostHpBarLifeTime--;

	//プレイヤーの体力が減った分を表す赤いバーを表示
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		m_lastPlayerHpBarEndPosX, static_cast<int>(playerHpBarEndPos.y),
		GetColor(255, 0, 0), true);
	//エネミーの体力が減った分を表す赤いバーを表示
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		m_lastEnemyHpBarEndPosX, static_cast<int>(enemyHpBarEndPos.y),
		GetColor(255, 0, 0), true);

	//赤いゲージが表示されて、一定時間ダメージを受けなかったら
	//プレイヤー側の処理
	if (m_playerLostHpBarLifeTime < 0)
	{
		//赤いゲージを短くしていく
		m_lastPlayerHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//現在の体力よりも短くなったら
		if (m_lastPlayerHpBarEndPosX > playerLostHpBoxPosX)
		{
			//現在の体力の座標に合わせる
			m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		}
	}
	//エネミー側の処理
	if (m_enemyLostHpBarLifeTime < 0)
	{
		//赤いゲージを短くしていく
		m_lastEnemyHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//現在の体力よりも短くなったら
		if (m_lastEnemyHpBarEndPosX > enemyLostHpBoxPosX)
		{
			//現在の体力の座標に合わせる
			m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
		}
	}
	//プレイヤーのHPバーの縁表示
	DrawRotaGraph(static_cast<int>(m_showUi[playerHpBar].drawPos.x + playerHpBarShakeSize.x),
		static_cast<int>(m_showUi[playerHpBar].drawPos.y + playerHpBarShakeSize.y),
		1.0, 0.0, m_showUi[playerHpBar].handle, true);
	//エネミーのHPバーの縁表示
	DrawRotaGraph(static_cast<int>(m_showUi[enemyHpBar].drawPos.x + enemyHpBarShakeSize.x),
		static_cast<int>(m_showUi[enemyHpBar].drawPos.y + +enemyHpBarShakeSize.y),
		1.0, 0.0, m_showUi[enemyHpBar].handle, true);

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
		//画像のロード
		pushData.handle = LoadGraph(("data/image/" + item.path + ".png").c_str());
		//画像の名前でマップに登録
		m_showUi[item.path] = pushData;
	}
}
