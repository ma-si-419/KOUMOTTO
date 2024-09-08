#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "Input.h"
#include "Game.h"
#include <cassert>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bool isWindow = true;


	// �E�B���h�E���t���X�N���[�����ςɂ���
	ChangeWindowMode(isWindow);

	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 16);

	SetMainWindowText("KOUMOTTO");

	SetChangeScreenModeGraphicsSystemResetFlag(false);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	Effekseer_Init(16000);
	//Effekseer_InitDistortion();
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Sync3DSetting();
	
	//�t�H���g�̓ǂݍ���
	LPCSTR fontPath = "data/toroman.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// �t�H���g�Ǎ��G���[����
		assert(0 && "�t�H���g�ǂݍ��ݎ��s");
	}

	ChangeFont("�A���j�����g���}��", DX_CHARSET_DEFAULT);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetDrawScreen(DX_SCREEN_BACK);


	SceneManager scene;
	DataManager data;
	SoundManager sound;
	
	data.LoadAttackFile();
	data.LoadUiFile();
	data.LoadAiFile();
	data.LoadSoundFile();
	MyEngine::Input input;
	scene.ChangeScene(std::make_shared<SceneTitle>(scene, data,sound));

	scene.Init();

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		//���͏��̍X�V
		input.Update();
		//�Z���N�g�{�^���������ꂽ��E�B���h�E���[�h��؂�ւ���
		if (input.IsTrigger("SELECT"))
		{
			isWindow = !isWindow;
			ChangeWindowMode(isWindow);
		}
		//�V�[���̍X�V
		scene.Update(input);
		//�V�[���̕`��
		scene.Draw();
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (scene.GetEnd())
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}
	}
	
	Effkseer_End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}