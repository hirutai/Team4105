#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	enum struct RotState : int {
		UP_LEFT = 0,
		UP,
		UP_RIGHT,
		RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT,
		LEFT
	};

	class Yankee : public Unit {
	private:
		Math::Point2 point_attack;

		float rotArray[8] = {
			-45.0f,0.0f,45.0f,90.0f,
			135.0f,180.0f,-135.0f,-90.0f,
		};

		Math::Point2 pointArray[8] = {
			Math::Point2(-1,1),Math::Point2(0,1),
			Math::Point2(1,1),Math::Point2(1,0),
			Math::Point2(1,-1),Math::Point2(0,-1),
			Math::Point2(-1,-1),Math::Point2(-1,0),
		};

	public:
		Yankee();
		~Yankee();
		// ê∂ê¨ä÷êî
		static std::shared_ptr<Yankee> Create(int point_x, int point_z);

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

