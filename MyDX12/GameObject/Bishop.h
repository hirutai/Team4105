#pragma once
#include "Unit.h"
#include <memory>

#include "../3D/Object3D.h"

namespace XIIlib {
	class Bishop : public Unit {
	private:
		Math::Point2 point_attack;
		Object3D* bike = nullptr;
	public:
		Bishop();
		~Bishop();
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Bishop> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void SetStartElement(int x, int z)override;
		void Action()override;
		void Attack()override;
		void Move()override;
		void AttackAreaDraw();
		bool AttackAreaExists()override;
		void IniState()override;

		void Hit(int attackPoint)override;

		void SetElementStock(int x, int z)override;
		void SetTypePositioning(_PositionType changeType)override;

		void CreateAttackArea()override;

		Math::Point2 GetElementStock()const { return element_stock; }

		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};

}
