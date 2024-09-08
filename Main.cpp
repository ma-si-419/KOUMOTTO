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


	// ウィンドウかフルスクリーンか可変にする
	ChangeWindowMode(isWindow);

	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 16);

	SetMainWindowText("KOUMOTTO");

	SetChangeScreenModeGraphicsSystemResetFlag(false);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	Effekseer_Init(16000);
	//Effekseer_InitDistortion();
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Sync3DSetting();
	
	//フォントの読み込み
	LPCSTR fontPath = "data/toroman.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// フォント読込エラー処理
		assert(0 && "フォント読み込み失敗");
	}

	ChangeFont("アンニャントロマン", DX_CHARSET_DEFAULT);

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

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		//入力情報の更新
		input.Update();
		//セレクトボタンが押されたらウィンドウモードを切り替える
		if (input.IsTrigger("SELECT"))
		{
			isWindow = !isWindow;
			ChangeWindowMode(isWindow);
		}
		//シーンの更新
		scene.Update(input);
		//シーンの描画
		scene.Draw();
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (scene.GetEnd())
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}
	
	Effkseer_End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}