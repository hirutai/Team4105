#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {

	class Bishop : public Unit {
	private:
		Math::Point2 point_attack;

		int intTimeCount = 0;

		int countingNum = 6; // カウントしたい数
	public:
		Bishop();
		~Bishop();
		// 生成関数
		static std::shared_ptr<Bishop> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void OriginBillDraw()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		void AttackAreaDraw();
		bool AttackAreaExists()override;
		void IniState()override;

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }

		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};

}
