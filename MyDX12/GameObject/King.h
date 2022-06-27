#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib{

	class King : public Unit {
	private:
		AREA type_attack = AREA::NONE;

		int moveCount;
		int moveLag;

		bool isDrawAtArea = false; // 攻撃しているかどうか
		int drawAtArea = 0;
	public:
		King();
		~King();

		// 生成関数
		static std::shared_ptr<King> CreateKing(int point_x, int point_z);

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void OriginBillDraw()override;

		void Move()override;
		void Attack()override;

		void Action()override;
		bool AttackAreaExists()override;
		void IniState()override;

		void CreateAttackArea()override;
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
		// 3Dオブジェクトの更新
		void ObjectUpdate()override;
	};
}