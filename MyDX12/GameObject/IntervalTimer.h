#pragma once

class DigitalNumberText;

// 座標
#define POSX 0
#define POSY 0
// 大きさ
#define SCALE 1.0f

// 内部カウンター
//#define INITIALCOUNTER 60 + 60 * MAXNUM // 初期カウント数
#define INITIALCOUNTER 60 + 60 * MAXNUM // 初期カウント数
#define ENDCOUNTER 0 // 終了カウント数

// 表示カウンター
#define MAXNUM DISPLAYTWO
#define DISPLAYTWO 2
#define DISPLAYONE (DISPLAYTWO - 1)
#define DISPLAYZERO (DISPLAYONE - 1)

namespace XIIlib
{
/// <summary>
/// 敵の行動間隔用タイマー
/// </summary>
	class IntervalTimer
	{
	public: // メンバ関数
		void Initialize(int textureNumber); // 初期化

		void Timer(); // タイマー関数

		void Draw(); // 描画

		const unsigned int GetTimer() { return internalCounter; } // タイマーの取得
		const unsigned int GetGameTimer() { return displayCounter; } // タイマーの取得

	private: // メンバ変数
		unsigned int internalCounter = INITIALCOUNTER; // 内部カウンター

		unsigned int displayCounter = MAXNUM; // 表示カウンター

		DigitalNumberText* timerNum = nullptr; // タイマー用の数字画像
	};
}
