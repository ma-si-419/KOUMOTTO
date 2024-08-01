#include "Ui.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	//HPバーの大きさ
	constexpr int kHpBarHeight = 15;
	constexpr int kHpBarWidth = 188;
	//MPバーの大きさ
	constexpr int kMpBarHeight = 8;
	constexpr int kMpBarWidth = 188;
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
	//プレイヤーのMPバーの座標(画像の座標に対してのMPバーの座標のずれを直す)
	constexpr int kMpBarPosX = -36;
	constexpr int kMpBarPosY = 40;

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

void Ui::DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy)
{
	//ダメージを受けた際プレイヤーのステートバーを揺らす大きさ
	MyEngine::Vector2 playerStateBarShakeSize(0, 0);
	//ダメージを受けた際エネミーのステートバーを揺らす大きさ
	MyEngine::Vector2 enemyStateBarShakeSize(0, 0);

	//前のフレームよりプレイヤーのHPが減っていたら
	if (m_lastPlayerHp > player->GetNowHp())
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_playerLostHpBarLifeTime = kLostHpBarLifeTime;

		//プレイヤーのステートバーの座標を揺らす
		playerStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}
	//前のフレームよりエネミーのHPが減っていたら
	if (m_lastEnemyHp > enemy->GetNowHp())
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_enemyLostHpBarLifeTime = kLostHpBarLifeTime;
		//エネミーのステートバーの座標を揺らす
		enemyStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}

	//体力量を覚えておく
	m_lastPlayerHp = player->GetNowHp();
	m_lastEnemyHp = enemy->GetNowHp();

	//画像の名前
	std::string playerStateBar = "PlayerStateBar";
	std::string enemyStateBar = "EnemyStateBar";

	//プレイヤーHPバーの開始位置
	MyEngine::Vector2 playerHpBarStartPos;
	playerHpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	playerHpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//プレイヤーHPバーの終了位置
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	playerHpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	//プレイヤーHPバーの長さ
	float playerHpBarLength = playerHpBarEndPos.x - playerHpBarStartPos.x;

	//プレイヤーMPバーの開始位置
	MyEngine::Vector2 playerMpBarStartPos;
	playerMpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kMpBarWidth + kMpBarPosX;
	playerMpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kMpBarHeight + kMpBarPosY;


	//プレイヤーMPバーの終了位置
	MyEngine::Vector2 playerMpBarEndPos;
	playerMpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kMpBarWidth + kMpBarPosX;
	playerMpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kMpBarHeight + kMpBarPosY;

	//プレイヤーMPバーの長さ
	float playerMpBarLength = playerMpBarEndPos.x - playerMpBarStartPos.x;

	//エネミーHPバーの開始位置
	MyEngine::Vector2 enemyHpBarStartPos;
	enemyHpBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	enemyHpBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//エネミーHPバーの終了位置
	MyEngine::Vector2 enemyHpBarEndPos;

	enemyHpBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	enemyHpBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	//エネミースタンバーの開始位置
	MyEngine::Vector2 enemyStanBarStartPos;


	//エネミースタンバーの終了位置
	MyEngine::Vector2 enemyStanBarEndPos;

	//プレイヤーのステートバーの座標を揺らす
	playerHpBarStartPos += playerStateBarShakeSize;
	playerHpBarEndPos += playerStateBarShakeSize;
	playerMpBarStartPos += playerStateBarShakeSize;
	playerMpBarEndPos += playerStateBarShakeSize;

	//エネミーのステートバーの座標を揺らす
	enemyHpBarStartPos += enemyStateBarShakeSize;
	enemyHpBarEndPos += enemyStateBarShakeSize;

	//プレイヤーのHPバー表示
	DrawBox(static_cast<int>(playerHpBarStartPos.x), static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 255, 64), true);
	//プレイヤーのMPバー表示
	DrawBox(static_cast<int>(playerMpBarStartPos.x), static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		GetColor(160, 192, 255), true);
	//エネミーのHPバーの表示
	DrawBox(static_cast<int>(enemyHpBarStartPos.x), static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 255, 64), true);


	//体力に応じて上にかぶせるボックスの長さを変化させる
	int playerLostHpBoxPosX = static_cast<int>(playerHpBarStartPos.x + playerHpBarLength * (player->GetNowHp() / player->GetStatus().hp));
	int enemyLostHpBoxPosX = static_cast<int>(enemyHpBarStartPos.x + enemyHpBarLength * (enemy->GetNowHp() / enemy->GetStatus().hp));

	//プレイヤーの気力に応じて上にかぶせるボックスの長さを変化させる
	int playerLostMpBoxPosX = static_cast<int>(playerMpBarStartPos.x + playerMpBarLength * (player->GetNowMp() / player->GetStatus().mp));

	//プレイヤーのHPバーの上にかぶせるボックス表示
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 64, 64), true);
	//エネミーのHPバーの上にかぶせるボックス表示
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 64, 64), true);

	//プレイヤーのMPバーの上にかぶせるボックス表示
	DrawBox(playerLostMpBoxPosX, static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
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
	//プレイヤーのステートバー表示
	DrawRotaGraph(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y),
		1.0, 0.0, m_showUi[playerStateBar].handle, true);
	//エネミーのステートバー表示
	DrawRotaGraph(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + +enemyStateBarShakeSize.y),
		1.0, 0.0, m_showUi[enemyStateBar].handle, true);
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
