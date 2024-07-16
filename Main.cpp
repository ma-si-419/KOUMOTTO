#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Input.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bool isWindow = true;


	// ウィンドウかフルスクリーンか可変にする
	ChangeWindowMode(isWindow);

	SetGraphMode(1280, 960, 16);


	SetChangeScreenModeGraphicsSystemResetFlag(false);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetUseZBuffer3D(true);
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager scene;
	MyEngine::Input input;
	scene.ChangeScene(std::make_shared<SceneTitle>(scene));

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

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}