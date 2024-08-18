#include "Ui.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	//HP�o�[�̑傫��
	constexpr int kHpBarHeight = 15;
	constexpr int kHpBarWidth = 188;
	//MP�o�[�̑傫��
	constexpr int kMpBarHeight = 8;
	constexpr int kMpBarWidth = 188;
	//�X�^���o�[�̑傫��
	constexpr int kStanBarHeight = 8;
	constexpr int kStanBarWidth = 128;
	//�̗͂����������̐Ԃ��Q�[�W���������Ă����܂ł̎���
	constexpr int kLostHpBarLifeTime = 30;
	//�Ԃ��Q�[�W���������鑬�x
	constexpr int kLostHpBarDecreaseSpeed = 3;
	//�_���[�W���󂯂����ɗh���傫��
	constexpr int kShakeScale = 15;
	//�_���[�W���󂯂����ɗh���傫���̔���
	constexpr int kShakeHalfScale = static_cast<int>(kShakeScale * 0.5);
	//HP�o�[�̍��W(�摜�̍��W�ɑ΂��Ă�HP�o�[�̍��W�̂���𒼂�)
	constexpr int kHpBarPosX = -52;
	constexpr int kHpBarPosY = 8;
	//�v���C���[��MP�o�[�̍��W(�摜�̍��W�ɑ΂��Ă�MP�o�[�̍��W�̂���𒼂�)
	constexpr int kMpBarPosX = -36;
	constexpr int kMpBarPosY = 40;
	//�G�l�~�[�̃X�^���o�[�̍��W(�摜�̍��W�ɑ΂��ẴX�^���o�[�̍��W�̂���𒼂�)
	constexpr int kStanBarPosX = 23;
	constexpr int kStanBarPosY = 40;
	//�_���[�W��\�����鎞��
	constexpr float kDamageShowTime = 30;
	//�_���[�W�\���������n�߂鎞��
	constexpr float kDamageVanishTime = 5;
	//�_���[�W��\��������W���������炷
	constexpr int kDamageShowPosShakeScale = 10;
	constexpr int kDamageShowPosShakeScaleHalf = 5;
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
	m_fontHandle = CreateFontToHandle("�A���j�����g���}��", 64, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

Ui::~Ui()
{
}

void Ui::Init()
{
}

void Ui::DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy)
{
	//�_���[�W���󂯂��ۃv���C���[�̃X�e�[�g�o�[��h�炷�傫��
	MyEngine::Vector2 playerStateBarShakeSize(0, 0);
	//�_���[�W���󂯂��ۃG�l�~�[�̃X�e�[�g�o�[��h�炷�傫��
	MyEngine::Vector2 enemyStateBarShakeSize(0, 0);

	//�O�̃t���[�����v���C���[��HP�������Ă�����
	if (m_lastPlayerHp > player->GetNowHp())
	{
		//����������\���Ԃ��o�[�������Ȃ�܂ł̎��Ԃ�ݒ肷��
		m_playerLostHpBarLifeTime = kLostHpBarLifeTime;

		//�v���C���[�̃X�e�[�g�o�[�̍��W��h�炷
		playerStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}
	//�O�̃t���[�����G�l�~�[��HP�������Ă�����
	if (m_lastEnemyHp > enemy->GetNowHp())
	{
		//����������\���Ԃ��o�[�������Ȃ�܂ł̎��Ԃ�ݒ肷��
		m_enemyLostHpBarLifeTime = kLostHpBarLifeTime;
		//�G�l�~�[�̃X�e�[�g�o�[�̍��W��h�炷
		enemyStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}

	//�̗͗ʂ��o���Ă���
	m_lastPlayerHp = player->GetNowHp();
	m_lastEnemyHp = enemy->GetNowHp();

	//�摜�̖��O
	std::string playerStateBar = "PlayerStateBar";
	std::string enemyStateBar = "EnemyStateBar";

	//�v���C���[HP�o�[�̊J�n�ʒu
	MyEngine::Vector2 playerHpBarStartPos;
	playerHpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	playerHpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//�v���C���[HP�o�[�̏I���ʒu
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	playerHpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	//�v���C���[HP�o�[�̒���
	float playerHpBarLength = playerHpBarEndPos.x - playerHpBarStartPos.x;

	//�v���C���[MP�o�[�̊J�n�ʒu
	MyEngine::Vector2 playerMpBarStartPos;
	playerMpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kMpBarWidth + kMpBarPosX;
	playerMpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kMpBarHeight + kMpBarPosY;


	//�v���C���[MP�o�[�̏I���ʒu
	MyEngine::Vector2 playerMpBarEndPos;
	playerMpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kMpBarWidth + kMpBarPosX;
	playerMpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kMpBarHeight + kMpBarPosY;

	//�v���C���[MP�o�[�̒���
	float playerMpBarLength = playerMpBarEndPos.x - playerMpBarStartPos.x;

	//�G�l�~�[HP�o�[�̊J�n�ʒu
	MyEngine::Vector2 enemyHpBarStartPos;

	enemyHpBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	enemyHpBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//�G�l�~�[HP�o�[�̏I���ʒu
	MyEngine::Vector2 enemyHpBarEndPos;

	enemyHpBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	enemyHpBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	//�G�l�~�[�X�^���o�[�̊J�n�ʒu
	MyEngine::Vector2 enemyStanBarStartPos;

	enemyStanBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kStanBarWidth + kStanBarPosX;
	enemyStanBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kStanBarHeight + kStanBarPosY;

	//�G�l�~�[�X�^���o�[�̏I���ʒu
	MyEngine::Vector2 enemyStanBarEndPos;

	enemyStanBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kStanBarWidth + kStanBarPosX;
	enemyStanBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kStanBarHeight + kStanBarPosY;

	float enemyStanBarLength = enemyStanBarEndPos.x - enemyStanBarStartPos.x;

	//�v���C���[�̃X�e�[�g�o�[�̍��W��h�炷
	playerHpBarStartPos += playerStateBarShakeSize;
	playerHpBarEndPos += playerStateBarShakeSize;
	playerMpBarStartPos += playerStateBarShakeSize;
	playerMpBarEndPos += playerStateBarShakeSize;

	//�G�l�~�[�̃X�e�[�g�o�[�̍��W��h�炷
	enemyHpBarStartPos += enemyStateBarShakeSize;
	enemyHpBarEndPos += enemyStateBarShakeSize;
	enemyStanBarStartPos += enemyStateBarShakeSize;
	enemyStanBarEndPos += enemyStateBarShakeSize;

	//�v���C���[��HP�o�[�\��
	DrawBox(static_cast<int>(playerHpBarStartPos.x), static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 255, 64), true);
	//�v���C���[��MP�o�[�\��
	DrawBox(static_cast<int>(playerMpBarStartPos.x), static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		GetColor(160, 192, 255), true);
	//�G�l�~�[��HP�o�[�̕\��
	DrawBox(static_cast<int>(enemyHpBarStartPos.x), static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 255, 64), true);
	//�G�l�~�[�̃X�^���o�[�\��
	DrawBox(static_cast<int>(enemyStanBarStartPos.x), static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		GetColor(192, 32, 192), true);


	//�̗͂ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int playerLostHpBoxPosX = static_cast<int>(playerHpBarStartPos.x + playerHpBarLength * (player->GetNowHp() / player->GetStatus().hp));
	int enemyLostHpBoxPosX = static_cast<int>(enemyHpBarStartPos.x + enemyHpBarLength * (enemy->GetNowHp() / enemy->GetStatus().hp));

	//�v���C���[�̋C�͂ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int playerLostMpBoxPosX = static_cast<int>(playerMpBarStartPos.x + playerMpBarLength * (player->GetNowMp() / player->GetStatus().mp));

	//�G�l�~�[�̃X�^���Q�[�W�ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int enemyLostStanBoxPosX = static_cast<int>(enemyStanBarStartPos.x + enemyStanBarLength * enemy->GetStanPointRate());

	//�v���C���[��HP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		GetColor(64, 64, 64), true);
	//�G�l�~�[��HP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		GetColor(64, 64, 64), true);

	//�v���C���[��MP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(playerLostMpBoxPosX, static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		GetColor(64, 64, 64), true);

	//�G�l�~�[�̃X�^���o�[�̏�ɂ��Ԃ���{�b�N�X��\��
	DrawBox(enemyLostStanBoxPosX, static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		GetColor(64, 64, 64), true);

	//�Ԃ��o�[�̏�����
	if (m_lastPlayerHpBarEndPosX == 0 && m_lastEnemyHpBarEndPosX == 0)
	{
		m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
	}

	//�Ԃ��o�[��������܂ł̎��Ԃ��J�E���g����
	m_playerLostHpBarLifeTime--;
	m_enemyLostHpBarLifeTime--;

	//�v���C���[�̗̑͂�����������\���Ԃ��o�[��\��
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		m_lastPlayerHpBarEndPosX, static_cast<int>(playerHpBarEndPos.y),
		GetColor(255, 0, 0), true);
	//�G�l�~�[�̗̑͂�����������\���Ԃ��o�[��\��
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		m_lastEnemyHpBarEndPosX, static_cast<int>(enemyHpBarEndPos.y),
		GetColor(255, 0, 0), true);

	//�Ԃ��Q�[�W���\������āA��莞�ԃ_���[�W���󂯂Ȃ�������
	//�v���C���[���̏���
	if (m_playerLostHpBarLifeTime < 0)
	{
		//�Ԃ��Q�[�W��Z�����Ă���
		m_lastPlayerHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//���݂̗̑͂����Z���Ȃ�����
		if (m_lastPlayerHpBarEndPosX > playerLostHpBoxPosX)
		{
			//���݂̗̑͂̍��W�ɍ��킹��
			m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		}
	}
	//�G�l�~�[���̏���
	if (m_enemyLostHpBarLifeTime < 0)
	{
		//�Ԃ��Q�[�W��Z�����Ă���
		m_lastEnemyHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//���݂̗̑͂����Z���Ȃ�����
		if (m_lastEnemyHpBarEndPosX > enemyLostHpBoxPosX)
		{
			//���݂̗̑͂̍��W�ɍ��킹��
			m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
		}
	}
	//�v���C���[�̃X�e�[�g�o�[�\��
	DrawRotaGraph(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y),
		1.0, 0.0, m_showUi[playerStateBar].handle, true);
	//�G�l�~�[�̃X�e�[�g�o�[�\��
	DrawRotaGraph(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + +enemyStateBarShakeSize.y),
		1.0, 0.0, m_showUi[enemyStateBar].handle, true);
}


void Ui::SetUiPos(std::string name, MyEngine::Vector2 pos)
{
	m_showUi[name].drawPos = pos;
}

void Ui::LoadUiHandle(std::vector<DataManager::UiInfo> data)
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

void Ui::AddShowDamage(MyEngine::Vector3 pos, int damage)
{
	DamageShowStatus pushData;
	VECTOR screenPos = ConvWorldPosToScreenPos(pos.CastVECTOR());
	pushData.pos = screenPos;
	//�\�����W���������炷
	pushData.pos.x += GetRand(kDamageShowPosShakeScale) - kDamageShowPosShakeScaleHalf;
	pushData.pos.y += GetRand(kDamageShowPosShakeScale) - kDamageShowPosShakeScaleHalf;
	pushData.damage = damage;
	pushData.time = kDamageShowTime;
	m_showDamage.push_back(pushData);
}

void Ui::DrawDamage()
{
	//�\�����Ԃ��I����Ă�����̂��폜����
	for (int i = 0; i < m_showDamage.size(); i++)
	{
		if (m_showDamage[i].time < 0)
		{
			m_showDamage.erase(m_showDamage.begin() + i);
			i--;
		}
	}
	//�`�揈��
	for (auto& item : m_showDamage)
	{
		int alpha = 0;
		//�\������c�莞�Ԃ����ȏ�̎��̓A���t�@�l�����炳�Ȃ�
		if (item.time > kDamageVanishTime)
		{
			alpha = 255;
		}
		//�c�莞�Ԃ����ȉ��ɂȂ�����
		else
		{
			//�c�莞�Ԃ���A���t�@�l���v�Z����
			alpha = 255 / kDamageVanishTime * item.time;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//�_���[�W�̕\��
		DrawFormatStringToHandle(item.pos.x, item.pos.y, GetColor(255, 255, 255), m_fontHandle, "%d", item.damage);
		//�u�����h���[�h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//�\�����Ԃ����炷
		item.time--;
	}
}
