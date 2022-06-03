#pragma once
#include "../Struct/Point2.h"
#include <vector>
#include <string>
#include"AttackAreaManager.h"
class CollisionCapsule;

namespace XIIlib {

	// �U���G���A
	enum struct AREA : int {
		NONE =-1,
		UP,
		UP_LEFT,
		LEFT,
		LEFT_DOWN,
		DOWN,
		DOWN_RIGHT,
		RIGHT,
		RIGHT_UP,
		MELEE
	};

	enum struct _PositionType : int {
		MINE = 0,
		ENEMY = 1
	};

	// ��̊��N���X
	class Unit {
	protected:
		unsigned int _cost; // ��ɂ�����R�X�g

		std::string ID; // �N���X�l�[��

		int _hit_point; // �̗�
		int _attack_point; // �U����
		int _defense_point; // �����

		int is_hit = 0;// 0 : �󂯂ĂȂ��A1 : �󂯂Ă���
		int damage_counter = 0;

		// �ǉ�
		unsigned int attackInterval = 180;
		bool isAttack = false;
		//�U��flag
		bool notAttackflag = true;
		Math::Point2 preElement_stock{0,0};

		bool is_dead = false;// ���S���Ă邩�ǂ������Ǘ�

		Math::Point2 element_stock;// �}�X�̍��W(Vector2����Ȃ���)

		std::vector<std::vector<Math::Point2>> attack_area; // ����U���͈͂̏��

		_PositionType type = _PositionType::MINE; // �G�������ʗp

		CollisionCapsule* collCapsule = nullptr; // ��̉����f��(�~)

		// �ǉ�
		Math::Point2 kingPos{0,0};

	public:
		Unit() = default;
		virtual ~Unit(){}

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		virtual void SetStartElement(int x, int z) = 0;		// �ŏ��̃}�X���W�̐ݒ�
		virtual void Move() = 0;							// �ړ��p
		virtual void Attack() = 0;							// �U���p(���͎g���ĂȂ�)
		
		virtual void Action() = 0;							// �s��
		virtual bool AttackAreaExists() = 0;                // �U���͈͂ɂ��邩
		virtual void Hit(int attackPoint) = 0;				// �_���[�W���󂯂�֐�
		virtual void IniState() = 0;

		virtual void SetElementStock(int x, int z) = 0;		// �}�X���W�̐ݒ�
		virtual void SetTypePositioning(_PositionType changeType) = 0;

		virtual void CreateAttackArea() = 0;

		// �ύX���܂����B
		int GetTypePositioning()const { return (int)type; }
		int IsDead()const { return is_dead; }

		void OnDead() { is_dead = true; }

		void ZeroHp() { if (_hit_point <= 0)OnDead(); }

		Math::Point2 GetElementStock()const { return element_stock; }

		int GetPatternElement(int pattern)const { return attack_area[pattern].size(); }

		Math::Point2 GetAttackArea(int pattern, int element)const { return attack_area[pattern][element]; }

		std::string GetID()const { return ID; }

		virtual bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum) = 0;

		bool ThreeCheckArea(Math::Point2 element_stock);
		
	};
}
