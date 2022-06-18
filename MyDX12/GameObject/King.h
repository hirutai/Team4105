#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib{

	class King : public Unit {
	private:
		AREA type_attack = AREA::NONE;

		int moveCount;
		int moveLag;

	public:
		King();
		~King();

		// ê∂ê¨ä÷êî
		static std::shared_ptr<King> CreateKing(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void Move()override;
		void Attack()override;

		void Action()override;
		bool AttackAreaExists()override;
		void IniState()override;

		void SetTypePositioning(_PositionType changeType)override;

		void CreateAttackArea()override;
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}