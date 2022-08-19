#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {
	class Stone : public Unit {
	private:
		Math::Point2 point_attack;

		bool nodamage = false;
		int nodamageTime = 0;
		int defaultNodamageTime = 11;
	public:
		Stone();
		~Stone();
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Stone> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void CreateAttackArea()override;

		void Damage();

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

