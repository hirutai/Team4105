#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	// ボスのアクション状態
	enum class BossState {
		wait = 0, // 待機
		attack // 攻撃
	};

	// ボスのパターンタイプ
	enum class BossType {
		normal = 0, // 通常状態
		strong // 強い状態(怒モード)
	};

	// ボスクラス
	class Boss : public Unit {
	private: // メンバ定数
		const int DISPLAY_FRAME = 12; // 12フレーム間隔で表示
		const int MAX_ATTACK_FRAME = 300; // 攻撃最中の最大フレーム
		static const int MAX_TILE = 8; // タイルの最大数
		static const int METEORS_MAX = 15; // メテオ攻撃のメテオ数

	private: // メンバ変数
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		Object3D* object3d2 = nullptr;
		Object3D* carobj = nullptr;
		BossState bossState = BossState::wait;
		BossType bossType = BossType::normal;
		
		int count = 0;
		int numbersA[METEORS_MAX]; // ランダムメテオの座標格納配列A
		int numbersB[METEORS_MAX]; // ランダムメテオの座標格納配列B
		int switchingCount = 0;
		int tileNum = 0;
		bool harfHpFlag = false; // 体力が半分以下かどうか
		bool switching = false;//画像切り替えよう
		bool tileDeth[MAX_TILE] = {};

		int attackFrameCnt = 0; // 攻撃フレームのカウント

	public: // 継承クラス
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Action()override;
		void Attack()override;
		void Target();
		void Move()override;
		bool AttackAreaExists()override;
		void IniState()override;
		void SetHitDamage(int attackPoint)override;				// ダメージを受ける関数
		void CreateAttackArea()override;
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;

	public: // メンバ関数
		Boss(); // コンストラクタ
		~Boss(); // デストラクタ
		// 生成関数
		static std::shared_ptr<Boss> Create(int point_x, int point_z);
		// Attackエリアの描画
		void AttackAreaDraw();
		// Attackエリア表示の変数などを初期化(Attackエリアの初期化ではない)
		void InitAttackDisplay();
		// メテオが他のメテオと重なっていないか
		bool CheckMeteorArea(Math::Point2 meteorPos);
		// 盤面上のマス座標を返す
		Math::Point2 GetElementStock()const { return element_stock; }
	};
}

