#pragma once
#include "Unit.h"
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	class Boss : public Unit {
	private:
		Math::Point2 point_attack;
		AREA type_attack = AREA::NONE;
		const int defaultHp = 6;
		BillObj* hpBar = nullptr; // HP�o�[�̃I�u�W�F�N�g
		BillObj* hpEdge = nullptr; // HP�̉��̃I�u�W�F�N�g

	public:
		Boss();
		~Boss();
		// �����֐�
		static std::shared_ptr<Boss> Create(int point_x, int point_z);

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

