#pragma once
#include "Boss.h"
#include "../Struct/Math.h"
#include <string>

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
		static const int SHOW_TILE_SPACE = 7; // 7フレーム間隔で表示
		static const int HIDE_TILE_SPACE = 7; // 7フレーム間隔で非表示
		static const int MAX_TILE = 8; // タイルの最大数
		static const int METEORS_MAX = 15; // メテオ攻撃のメテオ数
		const Math::Point2 METEOR_MIN_MAX = {0,7};
		const Math::Point2 BOSSTILE_MIN_MAX = { 0,5 };

	private: // メンバ変数
		int positionType = 0; // PositionTypeをint型で管理
		int frameCount = 0; // カウント
		int numbersA[METEORS_MAX] = {}; // ランダムメテオの座標格納配列A
		int numbersB[METEORS_MAX] = {}; // ランダムメテオの座標格納配列B
		int tileNum = 0; // タイルナンバー
		bool tilesShowFlag[MAX_TILE] = {}; // タイル表示の生死
		bool tileSwitch = true; // タイルを表示するか非表示にするか
		int attackFrameCnt = 0; // 攻撃フレームのカウント
		int meteorsCount = -20; // メテオsのカウント
		int targetTileRand = 0; // Line攻撃時のランダム座標格納

	public: // 静的関数
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
		/// タイルの表示遷移用
		/// </summary>
		void DispTileDeathControl(const int& bossAttackSelect);
		/// <summary>
		/// Line決めの核部分の座標をランダム生成
		/// </summary>
		void Target();
		/// <summary>
		/// ランダムなメテオを生成(重複なし)
		/// </summary>
		void CreateMeteorPosition(int& bossAttackSelect);
		/// <summary>
		/// メテオが他のメテオと重なっていないかチェック
		/// </summary>
		bool CheckMeteorArea(const Math::Point2& meteorPos);

		/// <summary>
		/// PositionTypeを代入
		/// </summary>
		/// <param name="type"></param>
		void AssignPositionType(const std::string& type);
	public: // Getter Setter
		int GetTargetTile() { return targetTileRand; }

	public: // ------------------ 表示攻撃選択系 ------------------
		/* Attackで攻撃 Displayで表示*/
		/// <summary>
		/// 縦方向3列表示
		/// </summary>
		void Vertical3Line(const std::string& type);
		/// <summary>
		/// 横方向3列表示
		/// </summary>
		void Horizontal3Line(const std::string& type);
		/// <summary>
		/// 1点 3x3メテオ
		/// </summary>
		void OneMeteor3x3(const std::string& type, const Math::Point2& kingPos);
	public: // 表示攻撃同時系
		/// <summary>
		/// ランダムまばら 1x1メテオ
		/// </summary>
		void RandomMeteor1x1Attack();
		// 近距離ノックバック攻撃(周囲1マス)
		void KnockBackAttack(const Math::Point2& pos);
	};
}


