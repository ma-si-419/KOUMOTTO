#include "LoadCsv.h"
#include <fstream>
#include <cassert>

//SpringGameをみて実装する

Game::AttackInfo LoadCsv::GetAttackData()
{
	Game::AttackInfo ans;
	//開くファイル名
	std::string path = "attackData.csv";
	//ファイル読み込み
	std::ifstream ifs(path.c_str());
	//読み込めなかったら止める
	if (!ifs)
	{
		assert(false);
	}
	//一行のデータを入れる
	std::string strBuf;
	//コンマ区切りのデータを入れる
	std::vector<std::string> strCommaBuf;

	//データをすべて読み込む
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
