#pragma once
#include <windows.h>

// プロトタイプ宣言
class WindowsApp;
class DirectX12;
class Sound;

namespace XIIlib
{
	class KeyInput;
	class ControllerInput;

	class Library
	{
	private:
		WindowsApp* w_app;
		DirectX12* dx12;
		KeyInput* keyInput;
		//ControllerInput* pad_input;
		const float MIN_FREAM_TIME = 1.0f / 60.0f;
		float frameTime = 0;
		LARGE_INTEGER timeStart;
		LARGE_INTEGER timeEnd;
		LARGE_INTEGER timeFreq;
		// fpsを取得するなら0で初期化しないとゴミが混ざってマイナスから始まったりする(かも知れない)
		float fps = 0;

	public:
		// コンストラクタ
		Library();
		// デストラクタ
		~Library();
		// 初期化
		int Initialize();
		// FPSの固定(簡易)
		int GetQuery();
		// Timerの破棄
		void RunTimer();
		// メッセージ
		bool Message();
		// 入力処理の更新
		void InputUpdate();
		// 描画開始
		void D_Start();
		// 描画終了
		void D_End();
	};
}