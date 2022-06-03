#pragma once
#include "../Struct/Point2.h"
class CollisionBox;

namespace XIIlib {

	class Tile {
	private:
		// ノックバックのベクトル
		Math::Point2 vec_back = Math::Point2();
		// 駒がいるかどうか
		bool on_tile = false;

		// 攻撃判定が有効化しているかどうか
		bool is_attack_player = false;
		bool is_attack_enemy = false;
		bool is_move_point = false;
		bool is_attack_point = false;

		// 攻撃判定の有無を計る
		int time_valid = 0; // 10f目で消える
		// マス用の仮モデル
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