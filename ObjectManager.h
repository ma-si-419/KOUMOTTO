#pragma once
#include <memory>
#include <vector>
#include "LoadCsv.h"
#include "Game.h"
#include <map>
#include "Vector3.h"

class ObjectBase;
class ObjectManager
{
public:
	ObjectManager();
	virtual ~ObjectManager();

	void LoadData(Game::SceneNum scene);

	void SetObject();

	void Draw();

private:
	struct ObjectData
	{
		std::string path;
		MyEngine::Vector3 pos;
		MyEngine::Vector3 rota;
		MyEngine::Vector3 scale;
	};

	enum class ObjectDataInfo
	{
		kName,
		kPath,
		kPosX,
		kPosY,
		kPosZ,
		kRotaX,
		kRotaY,
		kRotaZ,
		kScaleX,
		kScaleY,
		kScaleZ,
		kScene
	};

	std::map<std::string, ObjectData> m_sceneObjectDatas;

	std::vector<std::shared_ptr<ObjectBase>> m_pObjects;
};

