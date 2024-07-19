#include "LoadCsv.h"
#include <fstream>
#include <cassert>

//SpringGame���݂Ď�������

void LoadCsv::LoadAttackData()
{
	//�J���t�@�C����
	std::string path = "data/attackData.csv";
	//�t�@�C���ǂݍ���
	std::ifstream ifs(path.c_str());
	//�ǂݍ��߂Ȃ�������~�߂�
	if (!ifs)
	{
		assert(false);
	}
	//��s�̃f�[�^������
	std::string strBuf;
	//�R���}��؂�̃f�[�^������
	std::vector<std::string> strCommaBuf;

	//��s�ڂ̃f�[�^�͂���Ȃ��̂ň�x�ǂݍ���ł���
	std::getline(ifs, strBuf);

	//�f�[�^�����ׂēǂݍ���
	while (std::getline(ifs, strBuf))
	{
		//�؂������f�[�^
		strCommaBuf = Split(strBuf, ',');

		//�����f�[�^
		Game::AttackInfo pushData;

		//�����f�[�^�ɏ�������
		pushData.cost = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kCost)]);
		pushData.damageRate = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kDamageRate)]);
		pushData.speed = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kSpeed)]);
		pushData.radius = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kRadius)]);
		pushData.lifeTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kLifeTime)]);
		pushData.attackNum = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kAttackNum)]);
		pushData.isLaser = static_cast<bool>(std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kLaser)]));
		pushData.isScatter = static_cast<bool>(std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kScatter)]));
		pushData.isTrack = static_cast<bool>(std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kTrack)]));
		pushData.attackTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kAttackTime)]);
		pushData.actionTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kActionTime)]);
		pushData.isEnergy = static_cast<bool>(std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kEnergy)]));
		

		//�f�[�^������
		m_attackData[strCommaBuf[static_cast<int>(AttackInfoSort::kId)]] = pushData;
	}
}
std::vector<std::string> LoadCsv::Split(const std::string& str, const char cutChar)
{
	std::vector<std::string> ans;

	//��؂�J�n�ʒu
	int first = 0;

	//0����ǂ��܂ŋ�؂�̂�
	int last = static_cast<int>(str.find_first_of(cutChar));

	while (first < str.size())
	{
		//��؂������̂����ʂɓ���Ă���
		std::string subStr(str, first, last - first);

		ans.push_back(subStr);

		/*��؂�ʒu�X�V*/

		//��؂�J�n�ʒu����؂�ŏI�ʒu�ɍX�V����
		first = last + 1;

		//��؂�J�n�ʒu�����؂镶����T���n�߁A��؂�ŏI�ʒu�����肷��
		last = static_cast<int>(str.find_first_of(cutChar, first));

		//��؂镶���������Ȃ�������
		if (last == std::string::npos)
		{
			//��؂�ŏI�ʒu�𕶎���̍Ō�ɂ���
			last = static_cast<int>(str.size());
		}

	}

	return ans;
}
