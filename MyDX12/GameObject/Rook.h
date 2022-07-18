#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {

	class SmokeManager;

	class Rook : public Unit {
	private:
		Math::Point2 point_attack;
		SmokeManager* sManager = nullptr;
		int intTimeCount = 0;

		int countingNum = 5; // ÉJÉEÉìÉgÇµÇΩÇ¢êî
	public:
		Rook();
		~Rook();
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Rook> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void OriginBillDraw()override;

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

