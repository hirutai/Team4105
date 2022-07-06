#pragma once

#include "../Struct/Math.h"

namespace XIIlib
{
	class BillObj;

	enum class CountType
	{
		FRAME = 0,
		SECOND,
	};
	/// <summary>
	/// 敵の攻撃タイマー
	/// </summary>
	class AttackTimer
	{
#pragma region エイリアス
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region 定数
		float countingNum = 1.0f; // 数えたい秒数

		const float maxSize = 3.0f; // 最大サイズ
		const float second = 60.0f; // 一秒分のフレーム数
		const float baseDecNum = maxSize / second; // 一秒時に減らす量
		float decNum; // 減らす量
#pragma endregion

#pragma region メンバ関数
	public:
		// コンストラクタ
		AttackTimer(int countingNum_, XIIlib::CountType type);
		// 初期化
		void Initialize();
		// タイマー関数
		void Timer();
		// 描画
		void Draw();
		// タイマーの取得
		const unsigned int GetTimer() { return timerNum; }
		// サイズの取得
		bool SizeZeroFlag();
		bool SizeThirdFlag();//三分の一
		bool SizeThirdBelowFlag();//三分の一以下
		// バーの座標の設定
		void SetPosition(const Vector3& position);
#pragma endregion

#pragma region メンバ変数
	private:
		unsigned int timerNum; // タイマー

		float currentSize; // 現在のサイズ

		BillObj* timerBar = nullptr; // タイマーバーのオブジェクト
		BillObj* timerEdge = nullptr; // タイマーの縁のオブジェクト

		Vector2 barSize{ 3.0f, 0.75f }; // バーのサイズ
		Vector2 edgeSize{ 3.5f, 1.0f }; // 縁のサイズ
#pragma endregion
	};
}