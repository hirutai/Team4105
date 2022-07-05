#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	class Boss : public Unit {
	private:
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		BillObj* hpBar = nullptr; // HPバーのオブジェクト
		BillObj* hpEdge = nullptr; // HPの縁のオブジェクト

		int count = 0;
	public:
		Boss();
		~Boss();
		// 生成関数
		static std::shared_ptr<Boss> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;

		void Action()override;
		void Attack()override;
		void Target();
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void SetHitDamage(int attackPoint)override;				// ダメージを受ける関数

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

