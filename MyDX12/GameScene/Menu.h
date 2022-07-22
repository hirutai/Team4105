#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>
class Sprite;

namespace XIIlib {

	class GameScene;
	// カーソル状態
	enum class CursorState : int
	{
		NONE = -1,
		RULE,
		CONTINUE,
		PLAYER_GUIDS,
		ENEMY_GUIDS,
		NEXT_SLECT,
		NEXT_TITLE,
	};

	// メニュー状態
	enum class MenuState : int
	{
		NONE = -1,
		RULE,
		CONTINUE,
		PLAYER_GUIDS,
		ENEMY_GUIDS,
		NEXT_SLECT,
		NEXT_TITLE,
	};

	enum class EasingState : int
	{
		NONE = 0,
		MOVE_IN,
		MOVE_OUT,
	};
	// メニューシーンのクラス
	class Menu : public SceneState {
	private: // メンバ定数
		const float MAX_EASING_COUNT = 20.0f; // Easingのマックスカウント(20)
		const float CURSOR_SPACE = 250.0f; // カーソルと文字の間隔
		const float SPACE = 100.0f; // 間隔
		const int  EASING_SPACE = 3; // 間隔
		const Math::Vector2 winSize = { 1280.0f,768.0f }; // 画面サイズ
		const Math::Vector2 winCenter = { winSize.x/2,winSize.y / 2 }; // 画面の中央
	public:
		static const int MAX_BUTTON = 6; // スプライトButtonの個数

	private:
		// メニュー内で使う変数宣言の場(Object/Sprite/時間/フラグ等)
		CursorState cursorState = CursorState::CONTINUE; 
		MenuState menuState = MenuState::NONE;
		EasingState easingState = EasingState::MOVE_IN; // 最初から動く
		// ルール
		std::unique_ptr<Sprite> rule;
		// ボタン画像
		std::unique_ptr<Sprite> spStageBG1; // 背景
		std::unique_ptr<Sprite> cursor; // カーソル
		std::unique_ptr<Sprite> buttons[MAX_BUTTON] = {}; // ボタン
		// 説明画像
		std::unique_ptr<Sprite> playerGuide; // 主人公の説明
		std::unique_ptr<Sprite> enemyGuides; // 敵の説明
		// その他
		float easingCounts[MAX_BUTTON] = {}; // ボタンの数分のカウンター
		bool menuExists = true; // メニューが開かれてるかどうかの状態
		bool cursorDisp = false; // カーソル表示
		float maxPosY = 0; // 最大値
		float minPosY = 0; // 最小値
		Math::Vector2 prevPos = {0.0f,0.0f};
		Math::Vector2 rulePos;

		const float VEL_X = -4.0f;
		const int moveTime = 180;
		const int switchRandiMin = -3;
		const int switchRandiMax = 3;
		int count = 0;
		Math::Vector2 switchRand = { 0,0 };
	public:
		Menu();
		~Menu();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // メンバ関数
		void EasingUpdate();
		void EasingMove(int i,EasingState easingState); // イージングで移動
		void CountsUpdate(); // 全てのカウントを更新
		void CountUpdate(int& count); // 個々のカウントを更新
		void CheckLimitPos();// 最小最大値を決める

		void MoveCursor(); // カーソル処理
		void CheckMenuState(); // メニューの状態を座標で決める
	};
}