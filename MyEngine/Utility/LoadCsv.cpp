#include "LoadCsv.h"
#include <fstream>
#include <cassert>

//SpringGame���݂Ď�������

Game::AttackInfo LoadCsv::GetAttackData()
{
	Game::AttackInfo ans;
	//�J���t�@�C����
	std::string path = "attackData.csv";
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

	//�f�[�^�����ׂēǂݍ���
	while (std::getline(ifs, strBuf))
	{
		
	}

	return ans;
}

std::vector<std::string> LoadCsv::Split(const std::string& str, const char cutChar)
{
	std::vector<std::string> ans;



	return ans;
}
