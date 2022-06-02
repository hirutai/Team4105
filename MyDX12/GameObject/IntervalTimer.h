#pragma once

#include "../Struct/Math.h"

class Sprite;

namespace XIIlib
{
	/// <summary>
	/// 敵の行動間隔用タイマー
	/// </summary>
	class IntervalTimer
	{
#pragma region エイリアス
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region 定数
		const int countingNum = 5; // 数える秒数
		const int intCountingNum = 60 * countingNum; // 数えるフレーム数
#pragma endregion

#pragma region メンバ関数
	public:
		void Initialize(const int& timerTexNum, const int& barTexNum); // 初期化

		void Timer(); // タイマー関数

		void Draw(); // 描画

		const unsigned int GetTimer() { return timerNum; } // タイマーの取得
#pragma endregion

#pragma region メンバ変数
	private:
		unsigned int timerNum = intCountingNum; // タイマー

		Sprite* timer = nullptr; // タイマー画像
		Sprite* timer_bar = nullptr; // タイマーのバー画像
#pragma endregion
	};
}