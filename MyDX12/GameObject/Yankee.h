#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {
	class AttackTimer;

	class Yankee : public Unit {
	private:
		Math::Point2 point_attack;
		float movingTimer = 0.0f;
		bool determinateMoveAction = false;
		Math::Point2 nextPoint;

		AttackTimer* attackTimer = nullptr; // 攻撃タイマー
	public:
		Yankee();
		~Yankee();
		// 生成関数
		static std::shared_ptr<Yankee> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void SetTypePositioning(_PositionType changeType)override;

		void CreateAttackArea()override;

		void BillObjectDraw()override;

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

