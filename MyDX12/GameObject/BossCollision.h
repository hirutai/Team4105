#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {
	class BossCollision : public Unit {
	private:
		Math::Point2 point_attack;
	public:
		BossCollision();
		~BossCollision();
		// 生成関数
		static std::shared_ptr<BossCollision> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;
		

		void CreateAttackArea()override;
		void SetHitDamage(float attackPoint)override;	

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

