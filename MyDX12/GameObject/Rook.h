#pragma once
#include "Unit.h"
#include <memory>

namespace XIIlib {
	class AttackTimer;

	class Rook : public Unit {
	private:
		Math::Point2 point_attack;

		AttackTimer* attackTimer = nullptr; // �U���^�C�}�[
		int countingNum = 5; // �J�E���g��������
	public:
		Rook();
		~Rook();
		// �����֐�
		static std::shared_ptr<Rook> Create(int point_x, int point_z);

		void Initialize()override;
		void Update()override;

		void Action()override;
		void Attack()override;
		void Move()override;
		bool AttackAreaExists()override;
		void AttackAreaDraw();
		void IniState()override;

		void CreateAttackArea()override;

		void BillObjectDraw()override;

		Math::Point2 GetElementStock()const { return element_stock; }
		bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)override;
	};
}

