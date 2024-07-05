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
	//�I�v�V�������G���h�E�B���h�E���J���Ă��Ȃ��Ƃ�
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	/*�E�B���h�E�̏󋵂ŏ�����ς���*/

	//�����J���Ă��Ȃ��Ƃ�
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
				//�Z���N�g�V�[���ɔ��
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager));
			}
			else if (m_selectItem == static_cast<int>(ItemKind::kOption))
			{
				//�I�v�V�����E�B���h�E���J��
				m_isOpenOption = true;
			}
			else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
			{
				//�G���h�E�B���h�E���J��
				m_isOpenEndWindow = true;
				//�{���ɏI�����邩�ǂ������u�������v�ɓ���Ă���
				m_isEnd = false;
			}
		}
	}
	//�I�v�V�����E�B���h�E���J���Ă���Ƃ�
	else if (m_isOpenOption)
	{
		//�߂�{�^�����������Ƃ�
		if (input.IsTrigger(Game::InputId::kCancel))
		{
			m_isOpenOption = false;
		}
	}
	//�G���h�E�B���h�E���J���Ă���Ƃ�
	else if (m_isOpenEndWindow)
	{

		//�{���ɏI�����邩�ǂ�����I������
		if (input.IsTrigger(Game::InputId::kLeft))
		{
			m_isEnd = true;
		}
		else if (input.IsTrigger(Game::InputId::kRight))
		{
			m_isEnd = false;
		}

		//�߂�{�^�����������Ƃ�
		if (input.IsTrigger(Game::InputId::kCancel))
		{
			m_isOpenEndWindow = false;
		}

		//����{�^�����������Ƃ�
		if (input.IsTrigger(Game::InputId::kOk))
		{
			//�{���ɕ��邩�ǂ������m�F
			if (m_isEnd)
			{
				//�Q�[�����I������
				m_sceneManager.GameEnd();
			}
			else
			{
				//�E�B���h�E�����
				m_isOpenEndWindow = false;
			}
		}
	}
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", GetColor(255, 255, 255));
	//�I�v�V�������G���h�E�B���h�E���J���Ă��Ȃ��Ƃ�
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	//�E�B���h�E�������J���Ă��Ȃ��Ƃ�
	if (isCloseWindow)
	{
#ifdef _DEBUG

		DrawString(200, 200, "�͂��߂�", GetColor(255, 255, 255));
		DrawString(200, 300, "�I�v�V����", GetColor(255, 255, 255));
		DrawString(200, 400, "�I���", GetColor(255, 255, 255));
		DrawCircle(150, 200 + m_selectItem * 100, 20, GetColor(255, 0, 0), true);

#endif
	}
	//�I�v�V�������J���Ă���Ƃ�
	if (m_isOpenOption)
	{
#ifdef _DEBUG
		DrawString(500, 500, "���Ղ���񂾂�", GetColor(255, 255, 255));
		DrawString(500, 700, "B�Ŗ߂��", GetColor(255, 255, 255));
#endif
	}
	//�G���h�E�B���h�E���J���Ă���Ƃ�
	if (m_isOpenEndWindow)
	{
#ifdef _DEBUG
		DrawString(200, 200, "�{���ɏI�����܂���", GetColor(255, 255, 255));
		DrawString(100, 400, "�͂�", GetColor(255, 255, 255));
		DrawString(300, 400, "������", GetColor(255, 255, 255));
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