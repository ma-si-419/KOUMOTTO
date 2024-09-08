#pragma once
#include <map>
#include <string>
#include <vector>
class SoundManager
{
public:
	void SetHandle(std::vector<std::string> datas);

	void Play(std::string name,int PlayType);

	void Stop(std::string name);

private:
	std::map<std::string, int> m_soundHandles;
};