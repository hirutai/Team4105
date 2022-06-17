#include "Base/Library.h"
#include "Scene/GameApp.h"
#include "2D/PostEffect.h"

#include "Input/KeyInput.h"
#include <sstream>

// ターゲットがDebugの時
#ifdef _DEBUG
int main()
// ターゲットがDebugでない時
#else

// Windows アプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	XIIlib::Library* dxXii = nullptr;
	dxXii = new XIIlib::Library();
	dxXii->Initialize();

	dxXii->SetFPS();

	GameApp* app = nullptr;
	PostEffect* postEffect = nullptr;
	//// win_appのアドレス
	////printf("win:%x\n", (unsigned long int)win_app);

	// ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	// ゲームの初期化
	app = new GameApp();
	app->Initialize();
	Math::Vector2 windowSize = { WindowsApp::window_width, WindowsApp::window_height };

	postEffect->SetPosition(windowSize / 2);

	DWORD timeBefore = GetTickCount();
	DWORD fps = 0;

	while (true)
	{
		// メッセージ処理
		if (dxXii->Message()) { break; }

		++fps;

		if ((GetTickCount() - timeBefore) >= 1000) { // 1秒以上経過している
#ifdef _DEBUG // デバッグ用(デバッガにFSP出す)
#ifdef UNICODE
			std::wstringstream stream;
#else
			std::stringstream stream;
#endif
			stream << fps << " FPS" << std::endl;
			OutputDebugString(stream.str().c_str());
			std::wcout << stream.str()<< std::endl;
#endif

			// 0にリセット
			fps = 0;
			// リセット
			timeBefore = GetTickCount();
		}

		// 入力処理 : 更新
		dxXii->InputUpdate();

		// ゲームの毎フレーム処理(Update)
		app->Update();

		// 描画
		// 通常描画シーン
		postEffect->PreDrawScene(DirectX12::GetCmdList());
		app->LineDraw3D();

		//app->Draw();
		app->ObjMDraw();
		postEffect->PostLDrawScene(DirectX12::GetCmdList());
		postEffect->PostDrawScene(DirectX12::GetCmdList());

		// 開始
		dxXii->D_Start();
		app->BackSc();
		 //ポストエフェクトの描画
		postEffect->Draw(DirectX12::GetCmdList());
		app->DrawImGui();
		app->FrontSc();
		// 終了
		dxXii->D_End();
		app->DestoryAfter();

		// ESCで閉じる
		if (XIIlib::KeyInput::GetInstance()->Trigger(DIK_ESCAPE))break;
	}

	dxXii->ReleaseTimer();

	// 各種解放
	delete postEffect;
	delete app;

	delete dxXii;

	return 0;
}