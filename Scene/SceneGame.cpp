#include "Game.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "Physics.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "AttackBase.h"
#include "LoadCsv.h"

SceneGame::SceneGame(SceneManager& sceneManager) : SceneBase(sceneManager)
{
	handle = MV1LoadModel("data/model/Dome.mv1");
	//�����蔻��Ǘ��N���X�̃|�C���^
	m_pPhysics = std::make_shared<Physics>();
	//�v���C���[�̃|�C���^
	m_pPlayer = std::make_shared<Player>();
	//�J�����̃|�C���^
	m_pCamera = std::make_shared<Camera>();
	//�G�l�~�[�̃|�C���^
	m_pEnemy = std::make_shared<Enemy>();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	MV1SetPosition(handle, VGet(0, 0, 0));
	MV1SetScale(handle, VGet(500, 500, 500));


	//�v���C���[�̏�����(�����蔻���o�^����)
	m_pPlayer->Init(m_pPhysics);
	//�G�l�~�[�̏�����(�����蔻���o�^����)
	m_pEnemy->Init(m_pPhysics);
	//�J�����Ƀv���C���[�̍��W��n��
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����̏�����
	m_pCamera->Init();

	//�O���f�[�^
	LoadCsv data;

	//�K�E�Z�̃f�[�^�����[�h����
	data.LoadAttackData();
	//�v���C���[�ɕK�E�Z�̃f�[�^������
	m_pPlayer->SetAttackData(data.GetAttackData());
	//�G�l�~�[�ɕK�E�Z�̃f�[�^������
	m_pEnemy->SetAttackData(data.GetAttackData());
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
	//�J�����Ƀv���C���[�̍��W��n��
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����Ƀv���C���[���G�𒆐S�ɂǂ̂��炢��]���Ă��邩��n��
	m_pCamera->SetPlayerRota(m_pPlayer->GetRota());
	//�J�����̍X�V
	m_pCamera->Update();

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
	printfDx("size:%d,capacity%d\n", m_pAttacks.size(),m_pAttacks.capacity());

	if (input.IsTrigger(Game::InputId::kPause))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager));
		return;
	}
}

void SceneGame::Draw()
{
	MV1DrawModel(handle);

	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pPhysics->DebugDraw();
	DrawString(0, 0, "SceneGame", GetColor(255, 255, 255));
}

void SceneGame::End()
{
}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttacks.push_back(pAttack);
}
