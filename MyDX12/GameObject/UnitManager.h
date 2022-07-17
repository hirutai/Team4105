#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Unit.h"

class GamePAD_XInput;

namespace XIIlib {

	class BoardMap;
	class Audio;

	class UnitManager {
	private:
		UnitManager();
		~UnitManager();

		std::vector<std::shared_ptr<Unit>> units;
		std::vector<std::shared_ptr<Unit>> add_units;

		BoardMap* map_board = nullptr;

		int deadCount = 0;

		Audio* audio_ = nullptr;
		GamePAD_XInput* gamePad_ = nullptr;

	public:
		UnitManager(const UnitManager& obj) = delete;
		UnitManager& operator = (const UnitManager& obj) = delete;

		static UnitManager* GetInstance();

		void Initialize(); // 初期化

		void Update(); // 更新

		void Draw(); // 描画

		void BillDraw();

		void AddUnit(const std::shared_ptr<Unit>& spObject); // ユニットの追加

		void DeadUnit();

		// コンテナからすべて削除
		void AllDestroy();

		// 指定マスにユニットが載っているか?
		bool AllOnUnit(const Math::Point2& point);
		// そのマスにプレイヤーが存在するか否か
		bool IsPlayerOnTile(const Math::Point2& point);

		// 指定文字列からユニットの"ID(クラス名)"がヒットした要素数を返す
		int GetUnitIDElements(std::string name)const;
		// ユニットを取得
		std::shared_ptr<Unit> GetUnit(int index)const;
		// 複数の要素数を取得
		std::vector<int> GetUnitNumbers(std::string name)const;

		void SetBackVector(const Math::Point2& element, const Math::Point2& point);

		void FlatTileState();

		// 指定マスの攻撃判定を有効化
		void ChangeAttackValidTile(const Math::Point2& element, int type_positioning);

		void SetOnTile(const Math::Point2& element);

		void SetAudio(Audio* audio){ this->audio_ = audio; }

		bool IsAttackValid(const Math::Point2& element, int type_positioning)const;

		Math::Point2 GetBackVector(const Math::Point2& element)const;

		bool GetOnTile(const Math::Point2& element)const;

		std::vector<int> GetOnUnitElements(const Math::Point2& point);

		Audio* GetAudio() { return audio_; };//音

		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; }
		GamePAD_XInput* GetGamePad() {return gamePad_; }

		// 管理しているユニット数(全て)
		int GetAllUnitCount()const;

		// 敵が死んだ時にカウント関数
		void AddCountDead();
		// カウントのリセット
		void ResetCount();
		// カウントのゲッター
		int GetDeadCount()const { return deadCount; }
		// Object3Dの更新
		void ObjectUpdate();
	};

}