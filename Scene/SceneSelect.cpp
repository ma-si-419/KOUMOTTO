#include "Game.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneGame.h"

SceneSelect::SceneSelect(SceneManager& sceneManager, DataManager& dataManager) :
	SceneBase(sceneManager, dataManager),
	m_selectLevel(0),
	m_isBack(false)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

void SceneSelect::Update(MyEngine::Input input)
{
	if (input.IsTrigger(Game::InputId::kLeft))
	{
		if (m_selectLevel > 0)
		{
			m_selectLevel--;
		}
	}
	else if (input.IsTrigger(Game::InputId::kRight))
	{
		if (m_selectLevel < static_cast<int>(LevelKind::kHard))
		{
			m_selectLevel++;
		}
	}
	if (input.IsTrigger(Game::InputId::kUp))
	{
		m_isBack = false;
	}
	else if (input.IsTrigger(Game::InputId::kDown))
	{
		m_isBack = true;
	}

	if (input.IsTrigger("OK"))
	{
		if (m_isBack)
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
		}
		else
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager, m_dataManager));
		}
	}

}

void SceneSelect::Draw()
{
#ifdef _DEBUG
	DrawString(200, 400, "チュートリアル", GetColor(255, 255, 255));
	DrawString(400, 400, "イージー", GetColor(255, 255, 255));
	DrawString(600, 400, "ノーマル", GetColor(255, 255, 255));
	DrawString(800, 400, "ハード", GetColor(255, 255, 255));
	DrawString(500, 700, "もどる", GetColor(255, 255, 255));

	DrawFormatString(100, 0, GetColor(255, 255, 255), "%d", m_selectLevel);

	if (!m_isBack)
	{
		DrawCircle(200 + m_selectLevel * 200, 500, 20, GetColor(255, 0, 0), true);
	}
	else
	{
		DrawCircle(500, 700, 20, GetColor(255, 0, 0), true);
	}
#endif 

}

void SceneSelect::End()
{
}
