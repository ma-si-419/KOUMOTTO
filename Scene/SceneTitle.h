#pragma once
#include "SceneBase.h"
#include "DxLib.h"
class SceneTitle : public SceneBase
{
private:
	enum class ItemKind
	{
		kStart,
		kOption,
		kEnd
	};
	enum class OptionKind
	{
		kSE,
		kBGM,
		kWindowSize,
		kKindNum
	};
public:
	SceneTitle(SceneManager& sceneManager, DataManager& dataManager);
	~SceneTitle();
	//初期化処理
	void Init();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
private:
	//選択している項目
	int m_selectItem;
	//オプションを開いているか
	bool m_isOpenOption;
	//エンドウィンドウを開いているか
	bool m_isOpenEndWindow;
	//本当に閉じるかどうか
	bool m_isEnd;
};