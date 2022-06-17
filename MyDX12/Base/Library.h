#pragma once
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
		const unsigned int timerId = 1;
		const unsigned int frameRate = (1000 / 60);

	public:
		// コンストラクタ
		Library();
		// デストラクタ
		~Library();
		// 初期化
		int Initialize();
		// FPSの固定(簡易)
		void SetFPS();
		// Timerの破棄
		void ReleaseTimer();
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