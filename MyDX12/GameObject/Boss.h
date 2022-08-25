#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	enum class BossState {
		wait = 0,
		attack
	};
	enum class BossType {
		normal = 0,
		strong
	};
	class Boss : public Unit {
	private:
		const int DISPLAY_FRAME = 12;
		static const int MAX_TILE = 8;
		static const int METEORS_MAX = 15;
	private: // メンバ変数
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		Object3D* object3d2 = nullptr;
		BossState bossState = BossState::wait;
		BossType bossType = BossType::normal;
		
		int count = 0;
		int numbersA[METEORS_MAX];
		int numbersB[METEORS_MAX];
		int switchingCount = 0;
		bool harfHpFlag = false;
		bool switching = false;//画像切り替えよう
		bool tileDeth[MAX_TILE] = {};
		int tileNum = 0;

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
		void SetHitDamage(float attackPoint)override;				// ダメージを受ける関数
		void SetHitDamage(int attackPoint)override;				// ダメージを受ける関数
		void CreateAttackArea()override;

	public: // メンバ関数
		Boss();
		~Boss();
		// 生成関数
		static std::shared_ptr<Boss> Create(int point_x, int point_z);
		void AttackAreaDraw();
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
		void InitAttackDisplay();
		bool CheckMeteorArea(Math::Point2 meteorPos);
		Math::Point2 GetElementStock()const { return element_stock; }
	};
}

