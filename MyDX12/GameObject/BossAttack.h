#pragma once
#include "Boss.h"
#include "../Struct/Math.h"

// 今後の予定、攻撃と表示の関数あわせるかも

namespace XIIlib {
	// ボスアタッククラス
	class BossAttack
	{
	private: // シングルトン化
		BossAttack() = default;
		~BossAttack() = default;
		BossAttack(const BossAttack&) = delete;
		BossAttack& operator = (const BossAttack&) = delete;
	private: // 定数
		static const int DISPLAY_FRAME = 12; // 12フレーム間隔で表示
		static const int MAX_TILE = 8; // タイルの最大数
		static const int METEORS_MAX = 15; // メテオ攻撃のメテオ数

	private: // メンバ変数
		int count = 0;
		int numbersA[METEORS_MAX]; // ランダムメテオの座標格納配列A
		int numbersB[METEORS_MAX]; // ランダムメテオの座標格納配列B
		int tileNum = 0; // タイルナンバー
		bool tileDeth[MAX_TILE] = {}; // タイル表示の生死
		int attackFrameCnt = 0; // 攻撃フレームのカウント
		int meteorsCount = -20; // メテオsのカウント

		int bossMin = 0;
		int bossMax = 5;
		int bossTileRand = 0;

		int meteorAMin = 0;
		int meteorAMax = 7;
		int meteorBMin = 0;
		int meteorBMax = 7;

	public:
		/// <summary>
		/// 生成関数
		/// </summary>
		/// <returns>Instance</returns>
		static BossAttack* GetInstance();

	public: // メンバ関数
		/// <summary>
		/// Attackエリア表示の変数などを初期化(Attackエリアの初期化ではない)
		/// </summary>
		void InitAttackDisplay();
		/// <summary>
		/// Line決めの核部分の座標をランダム生成
		/// </summary>
		void Target();
		/// <summary>
		/// タイルの表示遷移用
		/// </summary>
		void DispTileDeathControl(const int& bossAttackSelect);
		/// <summary>
		/// ランダムなメテオを生成(重複なし)
		/// </summary>
		void CreateMeteorPosition();
		/// <summary>
		/// メテオが他のメテオと重なっていないかチェック
		/// </summary>
		bool CheckMeteorArea(const Math::Point2& meteorPos);

	public: // ------------------ 表示系 ------------------
		/// <summary>
		/// 縦方向3列表示
		/// </summary>
		void Vertical3LineDisplay();
		/// <summary>
		/// 横方向3列表示
		/// </summary>
		void Horizontal3LineDisplay();
		/// <summary>
		/// 1点 3x3メテオ
		/// </summary>
		void OneMeteor3x3Display(const Math::Point2& kingPos);
		/// <summary>
		/// ランダムまばら 1x1メテオ
		/// </summary>
		// void RandomMeteor1x1Display();

	public: // ------------------ 攻撃系 ------------------
		/// <summary>
		/// 縦方向3列攻撃
		/// </summary>
		void Vertical3LineAttack();
		/// <summary>
		/// 横方向3列攻撃
		/// </summary>
		void Horizontal3LineAttack();
		/// <summary>
		/// 1点 3x3メテオ
		/// </summary>
		void OneMeteor3x3Attack(const Math::Point2& kingPos);

	public: // 表示攻撃同時
		/// <summary>
		/// ランダムまばら 1x1メテオ
		/// </summary>
		void RandomMeteor1x1Attack();
		// 近距離ノックバック攻撃(周囲1マス)
		void KnockBackAttack(const Math::Point2& pos);
	};
}


