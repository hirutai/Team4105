#pragma once

#include "../Struct/Math.h"
#include"BossHP.h"

namespace XIIlib
{
	class BillObj;

	/// <summary>
	/// 敵の攻撃タイマー
	/// </summary>
	class HPBar
	{
#pragma region エイリアス
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region 定数
		const float maxSize = 60.0f; // 最大サイズ
#pragma endregion

#pragma region メンバ関数
	public:
		HPBar(const HPBar& obj) = delete;
		HPBar& operator = (const HPBar& obj) = delete;

		static HPBar* GetInstance();
		// コンストラクタ
		HPBar();
		// 初期化
		void Initialize();
		// タイマー関数
		void Timer(float damage);
		// 描画
		void Draw();
		// サイズの取得
		bool SizeZeroFlag();
		bool SizeThirdFlag();//三分の一
		bool SizeThirdBelowFlag();//三分の一以下
		// バーの座標の設定
		void SetPosition(const Vector3& position);
#pragma endregion

#pragma region メンバ変数
	private:
		float decNum; // 減らす量

		float currentSize; // 現在のサイズ

		BillObj* timerBar = nullptr; // タイマーバーのオブジェクト
		BillObj* timerEdge = nullptr; // タイマーの縁のオブジェクト

		Vector2 barSize{ maxSize, 1.0f }; // バーのサイズ
		Vector2 edgeSize{ maxSize + 1.0f, 2.0f }; // 縁のサイズ

		const Vector3 hpBarPos{ 0, 0, -25 }; // HPバーの座標
#pragma endregion
	};
}