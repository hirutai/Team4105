#pragma once
#include "../Struct/Point2.h"
class Object3D;

namespace XIIlib {

	class BillObj;

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
		bool is_attack_boss = false;

		// 攻撃判定の有無を計る
		int time_valid_player = 0; // 10f目で消える
		int time_valid_enemy = 0; // 10f目で消える
		int time_valid_move_point = 0; // 10f目で消える
		int time_valid_attack_point = 0; // 10f目で消える
		int time_valid_attack_boss = 0; // 10f目で消える
		// マス用のモデル
		Object3D* tile = nullptr;

		BillObj* warningTex = nullptr;
		BillObj* scullTex   = nullptr;
	public:
		static Tile* Create(float _x,float _z);

		Tile();
		~Tile();

		void Initialize(float _x, float _z);

		void AreaPlayer();
		void AreaEnemy();
		void AreaMovePoint();
		void AreaAttackPoint();
		void AreaAttackBoss();

		void Update();

		void Draw();

		void DrawBillObj();

		void SetPlayerAttackValid();
		void SetEnemyAttackValid();
		void SetMovePointValid();
		void SetAttackPointValid();
		void SetBossAttackValid();

		void SetBackVector(const Math::Point2& point);

		void SetOnTile();

		void Flat();

		bool IsPlayerAttackValid()const { return is_attack_player; }
		bool IsEnemyAttackValid()const { return is_attack_enemy; }
		bool IsMovePointValid()const { return is_move_point; }
		bool IsAttackPointValid()const { return is_attack_point; }
		bool IsBossAttackValid()const { return is_attack_boss; }

		Math::Point2 GetBackVector()const { return vec_back; }

		bool GetOnTile()const { return on_tile; }
	};

}