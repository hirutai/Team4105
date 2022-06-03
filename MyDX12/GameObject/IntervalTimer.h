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
		const float countingNum = 7; // 数えたい秒数

		const float maxSize = 300.0f; // 最大サイズ
		const float second = 60.0f; // 一秒分のフレーム数
		const float baseDecNum = maxSize / second; // 一秒時に減らす量
		const float decNum = baseDecNum / countingNum; // 減らす量
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
		int timerNum = 0; // タイマー

		float currentSize; // 現在のサイズ

		Sprite* timer = nullptr; // タイマー画像
		Sprite* timer_bar = nullptr; // タイマーのバー画像
#pragma endregion
	};
}