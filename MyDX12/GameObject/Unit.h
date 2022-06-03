#pragma once
#include "../Struct/Point2.h"
#include <vector>
#include <string>
#include"AttackAreaManager.h"
class CollisionCapsule;

namespace XIIlib {

	// 攻撃エリア
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

	// 駒の基底クラス
	class Unit {
	protected:
		unsigned int _cost; // 駒にかかるコスト

		std::string ID; // クラスネーム

		int _hit_point; // 体力
		int _attack_point; // 攻撃力
		int _defense_point; // 守備力

		int is_hit = 0;// 0 : 受けてない、1 : 受けている
		int damage_counter = 0;

		// 追加
		unsigned int attackInterval = 180;
		bool isAttack = false;
		//攻撃flag
		bool notAttackflag = true;
		Math::Point2 preElement_stock{0,0};

		bool is_dead = false;// 死亡してるかどうかを管理

		Math::Point2 element_stock;// マスの座標(Vector2じゃないよ)

		std::vector<std::vector<Math::Point2>> attack_area; // 駒が持つ攻撃範囲の情報

		_PositionType type = _PositionType::MINE; // 敵味方判別用

		CollisionCapsule* collCapsule = nullptr; // 駒の仮モデル(円)

		// 追加
		Math::Point2 kingPos{0,0};

	public:
		Unit() = default;
		virtual ~Unit(){}

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		virtual void SetStartElement(int x, int z) = 0;		// 最初のマス座標の設定
		virtual void Move() = 0;							// 移動用
		virtual void Attack() = 0;							// 攻撃用(今は使ってない)
		
		virtual void Action() = 0;							// 行動
		virtual bool AttackAreaExists() = 0;                // 攻撃範囲にいるか
		virtual void Hit(int attackPoint) = 0;				// ダメージを受ける関数
		virtual void IniState() = 0;

		virtual void SetElementStock(int x, int z) = 0;		// マス座標の設定
		virtual void SetTypePositioning(_PositionType changeType) = 0;

		virtual void CreateAttackArea() = 0;

		// 変更しますた。
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
