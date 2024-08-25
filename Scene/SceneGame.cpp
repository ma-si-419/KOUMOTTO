#include "Game.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "Physics.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Ui.h"
#include "AttackBase.h"
#include "LoadCsv.h"

SceneGame::SceneGame(SceneManager& sceneManager, DataManager& dataManager) :
	SceneBase(sceneManager, dataManager)
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
	m_pEnemy->SetUi(m_pUi);

	//�G�l�~�[�̍��W���v���C���[�ɓn��
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//�v���C���[�̏�����(�����蔻���o�^����)
	m_pPlayer->Init(m_pPhysics);
	//

	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����̏�����
	m_pGameCamera->Init();

	//�v���C���[�ɕK�E�Z�̃f�[�^������
	m_pPlayer->SetAttackData(m_dataManager.GetAttackData());
	//�G�l�~�[�ɕK�E�Z�̃f�[�^������
	m_pEnemy->SetAttackData(m_dataManager.GetAttackData());
	//UI�ɉ摜�̃f�[�^������
	m_pUi->LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kGame));
}

void SceneGame::Update(MyEngine::Input input)
{
	//�����蔻��̍X�V
	m_pPhysics->Update();
	//�v���C���[�̍X�V
	m_pPlayer->Update(shared_from_this(), input);
	//�G�l�~�[�̍X�V
	m_pEnemy->Update(shared_from_this());
	//�v���C���[�ɃG�l�~�[�̍��W��n��
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//�G�l�~�[�Ƀv���C���[�̍��W��n��
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����Ƀv���C���[���G�𒆐S�ɂǂ̂��炢��]���Ă��邩��n��
	m_pGameCamera->SetPlayerRota(m_pPlayer->GetRota());
	//�J�����̍X�V
	m_pGameCamera->Update();

	for (auto& attack : m_pAttacks)
	{
		//�U���̍X�V
		attack->Update(m_pEnemy->GetPos());
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

	if (input.IsTrigger(Game::InputId::kPause))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));
		return;
	}
}

void SceneGame::Draw()
{
	MV1DrawModel(handle);

	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pPhysics->DebugDraw();
	m_pUi->DrawStateBar(m_pPlayer, m_pEnemy);
	m_pUi->DrawDamage();
	DrawString(0, 0, "SceneGame", GetColor(255, 255, 255));
}

void SceneGame::End()
{
}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttacks.push_back(pAttack);
}
