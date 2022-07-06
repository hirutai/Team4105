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
		float countingNum = BossHP::GetInstance()->GetBossHP(); // 数えたいHP

		const float maxSize = 30.0f; // 最大サイズ
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
		void Timer(float attackPoint);
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

		Vector2 barSize{ 30.0f, 0.75f *5}; // バーのサイズ
		Vector2 edgeSize{ 30.5f, 1.0f *5}; // 縁のサイズ

		float sumPoint;
#pragma endregion
	};
}