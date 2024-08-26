#include "Game.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "TitleCamera.h"

namespace
{
	//�v���C���[�̏����ʒu
	MyEngine::Vector3 kPlayerInitPos(-500, -800, -500);
	//�G�l�~�[�̏����ʒu
	MyEngine::Vector3 kEnemyInitPos(500, -500, 500);
	//����h�炷�傫��
	constexpr float kShakeArrowScale = 10.0f;
	//����h�炷�X�s�[�h
	constexpr float kShakeArrowSpeed = 0.5f;
}

SceneTitle::SceneTitle(SceneManager& sceneManager, DataManager& dataManager) :
	SceneBase(sceneManager, dataManager),
	m_isEnd(false),
	m_isOpenEndWindow(false),
	m_isOpenOption(false),
	m_selectItem(0),
	m_domeHandle(-1),
	m_playerHandle(-1),
	m_enemyHandle(-1),
	m_shakeArrowNum(0)
{
	m_domeHandle = MV1LoadModel("data/model/Dome.mv1");
	m_playerHandle = MV1LoadModel("data/model/Player.mv1");
	m_enemyHandle = MV1LoadModel("data/model/Enemy.mv1");
	m_pTitleCamera = std::make_shared <TitleCamera>();
	m_fontHandle = CreateFontToHandle("�A���j�����g���}��", 96, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

SceneTitle::~SceneTitle()
{
	MV1DeleteModel(m_domeHandle);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_enemyHandle);
	DeleteFontToHandle(m_fontHandle);
}

void SceneTitle::Init()
{
	//Ui�̃��[�h
	LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kTitle));
	//���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_domeHandle, VGet(0, 0, 0));
	MV1SetPosition(m_playerHandle, kPlayerInitPos.CastVECTOR());
	MV1SetPosition(m_enemyHandle, kEnemyInitPos.CastVECTOR());
	//���f���̃X�P�[���ݒ�
	MV1SetScale(m_domeHandle, VGet(500, 500, 500));
	MV1SetScale(m_playerHandle, VGet(3, 3, 3));
	MV1SetScale(m_enemyHandle, VGet(300, 300, 300));
	//�v���C���[�ƃG�l�~�[�����������`�ɂ���
	MV1SetRotationZYAxis(m_playerHandle, (kPlayerInitPos - kEnemyInitPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	MV1SetRotationZYAxis(m_enemyHandle, (kEnemyInitPos - kPlayerInitPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	//�J�����̏�����
	m_pTitleCamera->Init();
}

void SceneTitle::Update(MyEngine::Input input)
{
	//�I�����Ă��鍀�ڂ��w�����͏�ɗh�炵������
	m_shakeArrowNum += kShakeArrowSpeed;

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
				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
				return;
			}
			//else if (m_selectItem == static_cast<int>(ItemKind::kOption))
			//{
			//	//�I�v�V�����E�B���h�E���J��
			//	m_isOpenOption = true;
			//}
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
	//else if (m_isOpenOption)
	//{
	//	//�߂�{�^�����������Ƃ�
	//	if (input.IsTrigger(Game::InputId::kCancel))
	//	{
	//		m_isOpenOption = false;
	//	}
	//}
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
	m_pTitleCamera->Update();
}

void SceneTitle::Draw()
{
	//�w�i�̕`��
	MV1DrawModel(m_domeHandle);
	MV1DrawModel(m_playerHandle);
	MV1DrawModel(m_enemyHandle);
	DrawString(0, 0, "SceneTitle", GetColor(255, 255, 255));

	//�I�v�V�������G���h�E�B���h�E���J���Ă��Ȃ��Ƃ�
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	//�E�B���h�E�������J���Ă��Ȃ��Ƃ�
	if (isCloseWindow)
	{
		//�^�C�g�����S�̕\��
		std::string titleLogo = "TitleLogo";
		DrawGraph(m_showUi[titleLogo].drawPos.x, m_showUi[titleLogo].drawPos.y, m_showUi[titleLogo].handle, true);;
		//������̕\��
		DrawStringToHandle(500, 500, "�o�g���X�^�[�g", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(620, 650, "��߂�", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		//���̕\��
		if (m_selectItem == static_cast<int>(ItemKind::kStart))
		{
			DrawStringToHandle(400 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "��", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
		{
			DrawStringToHandle(520 + sinf(m_shakeArrowNum) * kShakeArrowScale, 650, "��", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
	}

	//�I�v�V�������J���Ă���Ƃ�
//	if (m_isOpenOption)
//	{
//#ifdef _DEBUG
//		DrawString(500, 500, "���Ղ���񂾂�", GetColor(255, 255, 255));
//		DrawString(500, 700, "B�Ŗ߂��", GetColor(255, 255, 255));
//#endif
//	}
	//�G���h�E�B���h�E���J���Ă���Ƃ�
	if (m_isOpenEndWindow)
	{
		DrawStringToHandle(450, 300, "�ق�Ƃ��ɂ�߂�H", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(380, 500, "�͂�", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(950, 500, "������", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		if (m_isEnd)
		{
			DrawStringToHandle(280 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "��", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else
		{
			DrawStringToHandle(850 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "��", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
	}
}

void SceneTitle::End()
{
}

void SceneTitle::LoadUiHandle(std::vector<DataManager::UiInfo> data)
{
	for (auto& item : data)
	{
		//�Q�[���V�[����UI�����[�h����
		UiStatus pushData;
		//�f�[�^������
		pushData.drawPos = MyEngine::Vector2(item.posX, item.posY);
		//�摜�̃��[�h
		pushData.handle = LoadGraph(("data/image/" + item.path + ".png").c_str());
		//�摜�̖��O�Ń}�b�v�ɓo�^
		m_showUi[item.path] = pushData;
	}
}
