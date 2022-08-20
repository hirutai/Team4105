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
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		Object3D* object3d2 = nullptr;
		
		int count = 0;
		const int METEORS_MAX = 15;
		int numbersA[15];
		int numbersB[15];
	public:
		Boss();
		~Boss();
		// 生成関数
		static std::shared_ptr<Boss> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void Action()override;
		void Attack()override;
		void Target();
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();

		bool switching = false;//画像切り替えよう
		int switchingCount = 0;

		bool harfHpFlag = false;
		BossState bossState = BossState::wait;
		BossType bossType = BossType::normal;

		void IniState()override;

		void SetHitDamage(int attackPoint)override;				// ダメージを受ける関数

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;

		void InitAttackDisplay();

		bool CheckMeteorArea(Math::Point2 meteorPos);
	private:
		// 変数
		bool tileDeth[8] = {};
		int tileNum = 0;
		const int DISPLAY_FRAME = 12;
		const int MAX_TILE = 8;
	};
}

