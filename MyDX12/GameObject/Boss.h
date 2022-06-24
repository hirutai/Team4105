#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {

	class Boss : public Unit {
	private:
		Math::Point2 point_attack;
		float movingTimer = 0.0f;
		bool determinateMoveAction = false;
		Math::Point2 nextPoint;

	public:
		Boss();
		~Boss();
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Boss> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

