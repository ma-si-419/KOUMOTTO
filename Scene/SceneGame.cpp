#include "Game.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "Physics.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Ui.h"
#include "AttackBase.h"
#include "LoadCsv.h"
#include "EffekseerForDXLib.h"

namespace
{
	//�Q�[���I�[�o�[���̑I�����̐�
	constexpr int kGameOverItemNum = 1;
	//�o�g���J�n�܂ł̎���
	constexpr int kStandByTime = 120;
}

SceneGame::SceneGame(SceneManager& sceneManager, DataManager& dataManager) :
	SceneBase(sceneManager, dataManager),
	m_isGameOver(false),
	m_gameOverSelectItemNum(0),
	m_isStartBattle(false),
	m_standByTime(0)
{
	handle = MV1LoadModel("data/model/Dome.mv1");
	//�����蔻��Ǘ��N���X�̃|�C���^
	m_pPhysics = std::make_shared<Physics>();
	//�v���C���[�̃|�C���^
	m_pPlayer = std::make_shared<Player>();
	//�J�����̃|�C���^
	m_pGameCamera = std::make_shared<GameCamera>();
	//�G�l�~�[�̃|�C���^
	m_pEnemy = std::make_shared<Enemy>();
	//Ui�̃|�C���^
	m_pUi = std::make_shared<Ui>();

	m_dataManager.LoadAnimationFile();
	m_dataManager.LoadEffekseerHandle();

	m_pPlayer->SetAnimationData(m_dataManager.GetAnimationData(), true);
	m_pEnemy->SetAnimationData(m_dataManager.GetAnimationData(), false);

	m_pPlayer->SetEffekseerHandle(m_dataManager.GetEffekseerHandle());
	m_pEnemy->SetEffekseerHandle(m_dataManager.GetEffekseerHandle());

}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	MV1SetPosition(handle, VGet(0, 0, 0));
	MV1SetScale(handle, VGet(500, 500, 500));

	//�G�l�~�[�̏�����(�����蔻���o�^����)
	m_pEnemy->Init(m_pPhysics);
	//�G�l�~�[��State�p�^�[���̏�����
	m_pEnemy->InitAiState(shared_from_this());
	m_pEnemy->SetUi(m_pUi);

	//�G�l�~�[�̍��W���v���C���[�ɓn��
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//�v���C���[�ɃV�[���̃|�C���^��n��
	m_pPlayer->SetGameScene(shared_from_this());
	//�v���C���[�̏�����(�����蔻���o�^����)
	m_pPlayer->Init(m_pPhysics);
	//�G�l�~�[�Ƀv���C���[�̍��W��n��
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());

	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����Ƀv���C���[�̈ړ��ʂ�n��
//	m_pGameCamera->SetPlayerVelo(m_pPlayer->GetVelo());
	//�J�����̏�����
	m_pGameCamera->Init();

	//�v���C���[�ɕK�E�Z�̃f�[�^������
	m_pPlayer->SetAttackData(m_dataManager.GetAttackData());
	//�G�l�~�[�ɕK�E�Z�̃f�[�^������
	m_pEnemy->SetAttackData(m_dataManager.GetAttackData());
	//�G�l�~�[��AI�̃f�[�^������
	m_pEnemy->SetAiData(m_dataManager.GetAiData());

	//UI�ɉ摜�̃f�[�^������
	m_pUi->LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kGame));
}

void SceneGame::RetryInit()
{
	m_pPlayer->RetryInit();
	m_pEnemy->RetryInit();
	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����Ƀv���C���[���G�𒆐S�ɂǂ̂��炢��]���Ă��邩��n��
	//m_pGameCamera->SetPlayerRota(m_pPlayer->GetRota());
	//�J�����̏�����
	m_pGameCamera->Init();
	m_isGameOver = false;
	m_isStartBattle = false;
	m_standByTime = 0;
}

void SceneGame::Update(MyEngine::Input input)
{
	//�o�g�����J�n���Ă��Ȃ��Ƃ��̏���
	if (!m_isStartBattle)
	{
		m_standByTime++;
		//��莞�ԑҋ@������
		if (m_standByTime > kStandByTime)
		{
			//�o�g�����J�n����
			m_isStartBattle = true;
		}

	}
	//�o�g���J�n��̏���
	else
	{

		//�Q�[���I�[�o�[���̏���
		if (m_isGameOver)
		{
			//�����
			if (input.IsTrigger(Game::InputId::kUp))
			{
				m_gameOverSelectItemNum--;
				if (m_gameOverSelectItemNum < 0)
				{
					m_gameOverSelectItemNum = 0;
				}
			}
			//������
			if (input.IsTrigger(Game::InputId::kDown))
			{
				m_gameOverSelectItemNum++;
				if (m_gameOverSelectItemNum > static_cast<int>(Ui::GameOverItem::kItemNum))
				{
					m_gameOverSelectItemNum = static_cast<int>(Ui::GameOverItem::kItemNum);
				}
			}
			//�������
			if (input.IsTrigger(Game::InputId::kOk))
			{
				if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kRetry))
				{
					RetryInit();
				}
				else if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kEnd))
				{
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
					return;
				}
			}
		}
		//�Q�[���I�[�o�[���Ă��Ȃ��Ƃ��̏���
		else
		{
			//�����蔻��̍X�V
			m_pPhysics->Update();
			//�v���C���[�̍X�V
			m_pPlayer->Update(shared_from_this(), input);
			//�G�l�~�[�̍X�V
			m_pEnemy->Update(shared_from_this());
			m_pEnemy->StateUpdate(m_pPlayer);
		}
		//�v���C���[�ɃG�l�~�[�̍��W��n��
		m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
		//�G�l�~�[�Ƀv���C���[�̍��W��n��
		m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
		//�J�����Ƀv���C���[�̍��W��n��
		m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
		//�J�����ɃG�l�~�[�̍��W��n��
		m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
		//�J�����Ƀv���C���[�̉�]�s���n��
		m_pGameCamera->SetPlayerRotaMat(m_pPlayer->GetModelRotaMatrix());
		//�J�����̍X�V
		m_pGameCamera->Update();
		for (auto& attack : m_pAttacks)
		{
			//�U���̍X�V
			if (attack->GetTag() == ObjectTag::kPlayerAttack)
			{
				attack->Update(m_pEnemy->GetPos());
			}
			else if (attack->GetTag() == ObjectTag::kEnemyAttack)
			{
				attack->Update(m_pPlayer->GetPos());
			}
			//���������Ȃ��U����������
			if (!attack->GetIsExist())
			{
				//�����蔻�������
				attack->Final(m_pPhysics);
			}
		}

		for (int i = 0; i < m_pAttacks.size(); i++)
		{
			//�U���������Ă�����
			if (!m_pAttacks[i]->GetIsExist())
			{
				//�z�񂩂����
				m_pAttacks.erase(m_pAttacks.begin() + i);
				i--;
			}
		}
		printfDx("%d\n", m_pAttacks.size());


#ifdef _DEBUG
		if (input.IsTrigger(Game::InputId::kPause))
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager, m_dataManager));
			return;
		}
#endif
	}
	//�G�t�F�N�g���X�V����
	UpdateEffekseer3D();

}

void SceneGame::Draw()
{

	//�X�J�C�h�[���̕`��(������)
	MV1DrawModel(handle);

	//�v���C���[�̕`��
	m_pPlayer->Draw();
	//�G�l�~�[�̕`��
	m_pEnemy->Draw();
	//�G�t�F�N�g��`�悷��

#ifdef _DEBUG
	//�����蔻��̕`��
	m_pPhysics->DebugDraw();
#endif
	DrawEffekseer3D();
	//�v���C���[�ƃG�l�~�[�̗̑̓o�[��\������
	m_pUi->DrawStateBar(m_pPlayer, m_pEnemy);
	//�^�����_���[�W�̕\��
	m_pUi->DrawDamage();
	//�R�}���h�̕\��
	m_pUi->DrawCommand(m_pPlayer->GetIsOpenSpecialPallet(), m_pPlayer->GetSetSpecialAttackName());
	//�R���{���̕\��
	m_pUi->DrawComboCount();

	MyEngine::Vector3 pos = m_pGameCamera->GetPos();

	DrawFormatString(200,550,GetColor(0,0,0),"%f,%f,%f",pos.x,pos.y,pos.z);

	//�Q�[���I�[�o�[����UI�̕\��
	if (m_isGameOver)
	{
		m_pUi->DrawGameOver(m_gameOverSelectItemNum);
	}
	//�Q�[���J�n����Ready��Fight�̕\��
	m_pUi->DrawStartSign(m_isStartBattle);
}

void SceneGame::End()
{
	m_pPlayer->Final(m_pPhysics);
	m_pEnemy->Final(m_pPhysics);
	MV1DeleteModel(handle);

}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttacks.push_back(pAttack);
}