#pragma once
#include "../Struct/Point2.h"
#include <vector>
#include <string>
#include"AttackAreaManager.h"
#include "../Struct/Math.h"
class Object3D;

namespace XIIlib {
	class Audio;
	class AttackTimer;
	class BillObj;

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

		//乱数用
		//3以下しか動けない時の乱数
		int jMin = 1;
		int jMax = 3;
		int tileRand = 1;
		
		int SwitchRandiMin = 0;
		int SwitchRandiMax = 3;
		int SwitchRand = 0;

		unsigned int attackInterval = 180;
		//攻撃flag
		bool notAttackflag = true;
		bool isAttack = false;
		bool is_dead = false;// 死亡してるかどうかを管理

		Math::Point2 kingPos{0,0};
		Math::Point2 preElement_stock{0,0};
		Math::Point2 element_stock;// マスの座標(Vector2じゃないよ)
		std::vector<std::vector<Math::Point2>> attack_area; // 駒が持つ攻撃範囲の情報

		Audio* audio_ = nullptr;

		_PositionType type = _PositionType::MINE; // 敵味方判別用

		Object3D* object3d = nullptr;
		Object3D* daiza = nullptr;

		AttackTimer* attackTimer = nullptr; // 攻撃タイマー
		BillObj* attackAreasBillboard = nullptr;

		float movingTimer = 0.0f;
		bool determinateMoveAction = false;
		Math::Point2 nextPoint;
		Math::Vector3 pos;

		float correctionAngle = 0.0f;
	public:
		Unit() = default;
		virtual ~Unit(){}

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw();
		virtual void OriginBillDraw();

		virtual void Move() = 0;							// 移動用
		virtual void Attack() = 0;							// 攻撃用(今は使ってない)
		
		virtual void Action() = 0;							// 行動
		virtual bool AttackAreaExists() = 0;                // 攻撃範囲にいるか
		virtual void IniState() = 0;

		virtual void CreateAttackArea() = 0;

		void ZeroHp();

		virtual bool MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum) = 0;

		bool ThreeCheckArea(Math::Point2 element_stock);
		// Object3Dの更新
		virtual void ObjectUpdate();

	public:// Get関数群
		int GetPatternElement(int pattern)const { return attack_area[pattern].size(); }
		int GetTypePositioning()const { return (int)type; }
		int IsDead()const { return is_dead; }
		std::string GetID()const { return ID; }
		Math::Point2 GetElementStock()const { return element_stock; }
		Math::Point2 GetAttackArea(int pattern, int element)const { return attack_area[pattern][element]; }

	public:// Set関数群
		void SetTypePositioning(_PositionType changeType);
		void SetHitDamage(int attackPoint);				// ダメージを受ける関数
		void OnDead() { is_dead = true; }

		void SetAttackTimer(int countNum);

	public:// 共通関数
		void BillObjectDraw();

	protected:// 共通関数(private)
		void KnockBack();
		void Motion();
		void Direction(const Math::Point2& v);
		void SetElementStock(int x, int z);		// マス座標の設定
	};
}
