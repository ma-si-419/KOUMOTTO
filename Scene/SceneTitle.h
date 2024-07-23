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
	//����������
	void Init();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
private:
	//�I�����Ă��鍀��
	int m_selectItem;
	//�I�v�V�������J���Ă��邩
	bool m_isOpenOption;
	//�G���h�E�B���h�E���J���Ă��邩
	bool m_isOpenEndWindow;
	//�{���ɕ��邩�ǂ���
	bool m_isEnd;
};