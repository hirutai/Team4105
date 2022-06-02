#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {
	class Rook : public Unit {
	private:
		Math::Point2 point_attack;
	public:
		Rook();
		~Rook();
		//çUåÇflag
		bool notAttackflag = true;
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Rook> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void SetStartElement(int x, int z)override;
		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void Hit(int attackPoint)override;

		void SetElementStock(int x, int z)override;
		void SetTypePositioning(_PositionType changeType)override;

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }

	};
}

