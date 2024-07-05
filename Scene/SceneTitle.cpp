#include "Game.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager),
	m_isEnd(false),
	m_isOpenEndWindow(false),
	m_isOpenOption(false),
	m_selectItem(0)
{

}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Init()
{
}

void SceneTitle::Update(MyEngine::Input input)
{
	//オプションもエンドウィンドウも開いていないとき
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	/*ウィンドウの状況で処理を変える*/

	//何も開いていないとき
	if (isCloseWindow)
	{
		if (input.IsTrigger(Game::InputId::kUp))
		{
			if (m_selectItem > 0)
			{
				m_selectItem--;
			}
		}
		else if (input.IsTrigger(Game::InputId::kDown))
		{
			if (m_selectItem < static_cast<int>(ItemKind::kEnd))
			{
				m_selectItem++;
			}
		}
		if (input.IsTrigger(Game::InputId::kOk))
		{
			if (m_selectItem == static_cast<int>(ItemKind::kStart))
			{
				//セレクトシーンに飛ぶ
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager));
			}
			else if (m_selectItem == static_cast<int>(ItemKind::kOption))
			{
				//オプションウィンドウを開く
				m_isOpenOption = true;
			}
			else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
			{
				//エンドウィンドウを開く
				m_isOpenEndWindow = true;
				//本当に終了するかどうかを「いいえ」に入れておく
				m_isEnd = false;
			}
		}
	}
	//オプションウィンドウを開いているとき
	else if (m_isOpenOption)
	{
		//戻るボタンを押したとき
		if (input.IsTrigger(Game::InputId::kCancel))
		{
			m_isOpenOption = false;
		}
	}
	//エンドウィンドウを開いているとき
	else if (m_isOpenEndWindow)
	{

		//本当に終了するかどうかを選択する
		if (input.IsTrigger(Game::InputId::kLeft))
		{
			m_isEnd = true;
		}
		else if (input.IsTrigger(Game::InputId::kRight))
		{
			m_isEnd = false;
		}

		//戻るボタンを押したとき
		if (input.IsTrigger(Game::InputId::kCancel))
		{
			m_isOpenEndWindow = false;
		}

		//決定ボタンを押したとき
		if (input.IsTrigger(Game::InputId::kOk))
		{
			//本当に閉じるかどうかを確認
			if (m_isEnd)
			{
				//ゲームを終了する
				m_sceneManager.GameEnd();
			}
			else
			{
				//ウィンドウを閉じる
				m_isOpenEndWindow = false;
			}
		}
	}
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", GetColor(255, 255, 255));
	//オプションもエンドウィンドウも開いていないとき
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	//ウィンドウを何も開いていないとき
	if (isCloseWindow)
	{
#ifdef _DEBUG

		DrawString(200, 200, "はじめる", GetColor(255, 255, 255));
		DrawString(200, 300, "オプション", GetColor(255, 255, 255));
		DrawString(200, 400, "終わる", GetColor(255, 255, 255));
		DrawCircle(150, 200 + m_selectItem * 100, 20, GetColor(255, 0, 0), true);

#endif
	}
	//オプションを開いているとき
	if (m_isOpenOption)
	{
#ifdef _DEBUG
		DrawString(500, 500, "おぷしょんだよ", GetColor(255, 255, 255));
		DrawString(500, 700, "Bで戻るよ", GetColor(255, 255, 255));
#endif
	}
	//エンドウィンドウを開いているとき
	if (m_isOpenEndWindow)
	{
#ifdef _DEBUG
		DrawString(200, 200, "本当に終了しますか", GetColor(255, 255, 255));
		DrawString(100, 400, "はい", GetColor(255, 255, 255));
		DrawString(300, 400, "いいえ", GetColor(255, 255, 255));
		DrawCircle(300 - m_isEnd * 200, 400, 20, GetColor(255, 0, 0), true);
#endif
	}

#ifdef _DEBUG

	DrawFormatString(500, 0, GetColor(255, 255, 255), "%d", m_selectItem);
	DrawFormatString(300, 0, GetColor(255, 255, 255), "%d", m_isEnd);

#endif  

}

void SceneTitle::End()
{
}