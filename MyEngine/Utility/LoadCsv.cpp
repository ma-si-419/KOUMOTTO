#include "LoadCsv.h"
#include <fstream>
#include <cassert>

//SpringGameをみて実装する

void LoadCsv::LoadAttackData()
{
	//開くファイル名
	std::string path = "data/attackData.csv";
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

	//一行目のデータはいらないので一度読み込んでおく
	std::getline(ifs, strBuf);

	//データをすべて読み込む
	while (std::getline(ifs, strBuf))
	{
		//切り取ったデータ
		strCommaBuf = Split(strBuf, ',');

		//入れるデータ
		Game::AttackInfo pushData;

		//入れるデータに情報を入れる
		pushData.cost = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kCost)]);
		pushData.damageRate = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kDamageRate)]);
		pushData.speed = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kSpeed)]);
		pushData.radius = std::stof(strCommaBuf[static_cast<int>(AttackInfoSort::kRadius)]);
		pushData.lifeTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kLifeTime)]);
		pushData.attackNum = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kAttackNum)]);
		pushData.isLaser = static_cast<bool>(std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kLaser)]));
		pushData.trackStartTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kTrackStartTime)]);
		pushData.trackEndTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kTrackEndTime)]);
		pushData.actionTime = std::stoi(strCommaBuf[static_cast<int>(AttackInfoSort::kActionTime)]);

		//データを入れる
		m_attackData[strCommaBuf[static_cast<int>(AttackInfoSort::kId)]] = pushData;
	}
}
std::vector<std::string> LoadCsv::Split(const std::string& str, const char cutChar)
{
	std::vector<std::string> ans;

	//区切り開始位置
	int first = 0;

	//0からどこまで区切るのか
	int last = static_cast<int>(str.find_first_of(cutChar));

	while (first < str.size())
	{
		//区切ったものを結果に入れていく
		std::string subStr(str, first, last - first);

		ans.push_back(subStr);

		/*区切る位置更新*/

		//区切り開始位置を区切り最終位置に更新する
		first = last + 1;

		//区切り開始位置から区切る文字を探し始め、区切り最終位置を決定する
		last = static_cast<int>(str.find_first_of(cutChar, first));

		//区切る文字がもうなかったら
		if (last == std::string::npos)
		{
			//区切り最終位置を文字列の最後にする
			last = static_cast<int>(str.size());
		}

	}

	return ans;
}
