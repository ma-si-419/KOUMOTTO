#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneGame.h"

SceneSelect::SceneSelect(SceneManager& sceneManager) :
	SceneBase(sceneManager)
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
	if (input.IsTrigger("LEFT"))
	{
		if (m_selectLevel > 0)
		{
			m_selectLevel--;
		}
	}
	else if (input.IsTrigger("RIGHT"))
	{
		if (m_selectLevel < static_cast<int>(LevelKind::kHard))
		{
			m_selectLevel++;
		}
	}
	if (input.IsTrigger("UP"))
	{
		m_isBack = false;
	}
	else if (input.IsTrigger("DOWN"))
	{
		m_isBack = true;
	}

	if (input.IsTrigger("OK"))
	{
		if (m_isBack)
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager));
		}
		else
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager));
		}
	}

}

void SceneSelect::Draw()
{
	DrawString(200, 400, "チュートリアル", GetColor(255, 255, 255));
	DrawString(400, 400, "イージー", GetColor(255, 255, 255));
	DrawString(600, 400, "ノーマル", GetColor(255, 255, 255));
	DrawString(800, 400, "ハード", GetColor(255, 255, 255));
	DrawString(500, 700, "もどる", GetColor(255, 255, 255));

	if (!m_isBack)
	{
		DrawCircle(200 + m_selectLevel * 200, 500, 20, GetColor(255, 0, 0), true);
	}
	else
	{
		DrawCircle(500, 500, 20, GetColor(255, 0, 0), true);
	}
}

void SceneSelect::End()
{
}
