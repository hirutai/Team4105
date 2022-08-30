#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	class BossAttack;
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
	protected: // メンバ定数
		static const int MAX_ATTACK_FRAME = 300; // 攻撃最中の最大フレーム
		const Math::Point2 ATTACK_RANDOM_MIN_MAX = { 0,1 }; // アタックを制御する最小と最大
		const float COLISION_HEIGHT = 2.0f;

	protected: // メンバ変数
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		Object3D* object3d2 = nullptr; // 3DObject 盤長攻撃時
		Object3D* carobj = nullptr; // 3DObject 車
		BossState bossState = BossState::wait; // ボスの状態格納
		BossType bossType = BossType::normal; // ボスのタイプ格納

		int switchingCount = 0; // 切り替えるため間隔をカウント
		bool harfHpFlag = false; // 体力が半分以下かどうか
		bool switching = false; // 画像切り替えよう
		int attackSelect = 0; // ボスの攻撃行動を制御する (n:0=縦3Line,1=横3Line S:0=1点メテオ,1=ランダムメテオ)
		int attackFrameCnt = 0; // ボスの攻撃中のフレームカウント

	public: // 継承クラス
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void IniState()override;
		void SetHitDamage(int attackPoint)override;				// ダメージを受ける関数
		void CreateAttackArea()override;
		void ObjectUpdate()override;// 3Dオブジェクトの更新
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;

	public: // メンバ関数
		Boss(); // コンストラクタ
		~Boss(); // デストラクタ
		// 生成関数
		static std::shared_ptr<Boss> Create(int point_x, int point_z);
		
	};
}

