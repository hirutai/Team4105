#pragma once
#include "../Struct/Point2.h"
class CollisionBox;

namespace XIIlib {

	class Tile {
	private:
		// �m�b�N�o�b�N�̃x�N�g��
		Math::Point2 vec_back = Math::Point2();
		// ����邩�ǂ���
		bool on_tile = false;

		// �U�����肪�L�������Ă��邩�ǂ���
		bool is_attack_player = false;
		bool is_attack_enemy = false;
		bool is_move_point = false;
		bool is_attack_point = false;

		// �U������̗L�����v��
		int time_valid = 0; // 10f�ڂŏ�����
		// �}�X�p�̉����f��
		CollisionBox* p_box = nullptr;

	public:
		static Tile* Create(float _x,float _z);

		Tile();
		~Tile();

		void Initialize(float _x, float _z);

		void Update();

		void Draw();

		void SetPlayerAttackValid();
		void SetEnemyAttackValid();
		void SetMovePointValid();
		void SetAttackPointValid();

		void SetBackVector(const Math::Point2& point);

		void SetOnTile();

		bool IsPlayerAttackValid()const { return is_attack_player; }
		bool IsEnemyAttackValid()const { return is_attack_enemy; }
		bool IsMovePointValid()const { return is_move_point; }
		bool IsAttackPointValid()const { return is_attack_point; }

		Math::Point2 GetBackVector()const { return vec_back; }

		bool GetOnTile()const { return on_tile; }
	};

}