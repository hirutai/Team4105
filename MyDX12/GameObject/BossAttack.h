#pragma once
#include "Boss.h"
#include "../Struct/Math.h"

// 今後の予定、攻撃と表示の関数あわせるかも

namespace XIIlib {
	// ボスアタッククラス
	class BossAttack : public Boss
	{
	private: // シングルトン化
		BossAttack() = default;
		~BossAttack() = default;
		BossAttack(const BossAttack&) = delete;
		BossAttack& operator = (const BossAttack&) = delete;
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
		void DispTileDeathControl();
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
		void RandomMeteor1x1Display();

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
		/// <summary>
		/// ランダムまばら 1x1メテオ
		/// </summary>
		void RandomMeteor1x1Attack();

	public: // 表示攻撃同時
		// 近距離にいるplayerを吹き飛ばす
		void KnockBackAttack(const Math::Point2& pos);
	};
}


