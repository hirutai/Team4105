#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Unit.h"

namespace XIIlib {

	class BoardMap;
	class IntervalTimer;

	class UnitManager {
	private:
		UnitManager();
		~UnitManager();

		std::vector<std::shared_ptr<Unit>> units;
		std::vector<std::shared_ptr<Unit>> add_units;

		IntervalTimer* _intervalTimter = nullptr;// コピー用

		BoardMap* map_board = nullptr;

		int deadCount = 0;

	public:
		UnitManager(const UnitManager& obj) = delete;
		UnitManager& operator = (const UnitManager& obj) = delete;

		static UnitManager* GetInstance();

		void Initialize(); // 初期化

		void Update(); // 更新

		void Draw(); // 描画

		void AddUnit(const std::shared_ptr<Unit>& spObject); // ユニットの追加

		void DeadUnit();

		// コンテナからすべて削除
		void AllDestroy();

		// 指定マスにユニットが載っているか?
		bool AllOnUnit(const Math::Point2& point);

		// 指定文字列からユニットの"ID(クラス名)"がヒットした要素数を返す
		int GetUnitIDElements(std::string name)const;
		// ユニットを取得
		std::shared_ptr<Unit> GetUnit(int index)const;
		// 複数の要素数を取得
		std::vector<int> GetUnitNumbers(std::string name)const;

		void SetBackVector(const Math::Point2& element, const Math::Point2& point);

		// 指定マスの攻撃判定を有効化
		void ChangeAttackValidTile(const Math::Point2& element, int type_positioning);

		void SetOnTile(const Math::Point2& element);

		bool IsAttackValid(const Math::Point2& element, int type_positioning)const;

		Math::Point2 GetBackVector(const Math::Point2& element)const;

		bool GetOnTile(const Math::Point2& element)const;

		std::vector<int> GetOnUnitElements(const Math::Point2& point);

		// タイマーのGetSet関数
		/// <summary>
		/// IntervalTimer型を設定
		/// </summary>
		/// <param name="intervalTimer">インターバルタイム</param>
		void SetIntervalTimer(IntervalTimer* intervalTimer) { _intervalTimter = intervalTimer; }

		/// <summary>
		/// Timerの取得
		/// </summary>
		/// <returns> unsigned int タイマーを返す</returns>
		const unsigned int GetIntervalTimer();

		// 敵が死んだ時にカウント関数
		void AddCountDead();
		// カウントのリセット
		void ResetCount();

	};

}