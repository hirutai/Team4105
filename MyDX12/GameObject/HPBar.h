#pragma once

#include "../Struct/Math.h"
#include"BossHP.h"

class Sprite;

namespace XIIlib
{
	//class BillObj;

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
		const int maxSize = 30 * 40; // 最大サイズ
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
		void Timer(int damage);
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
		int decNum; // 減らす量

		int currentSize; // 現在のサイズ

		//BillObj* timerBar = nullptr; // タイマーバーのオブジェクト
		//BillObj* timerEdge = nullptr; // タイマーの縁のオブジェクト
		Sprite* timerBar = nullptr; // タイマーバーのオブジェクト
		Sprite* timerEdge = nullptr; // タイマーの縁のオブジェクト

		Vector2 barSize{ maxSize, 30.0f }; // バーのサイズ
		Vector2 edgeSize{ maxSize + 20.0f, 50.0f }; // 縁のサイズ

		const Vector2 hpBarPos{ 40, 690.0f }; // HPバーの座標
#pragma endregion
	};
}