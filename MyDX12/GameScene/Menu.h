#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
	// メニュー状態
	enum class MenuState : int
	{
		NONE = -1,
		CONTINUE,
		PLAYER_GUIDS,
		ENEMY_GUIDS,
		NEXT_SLECT,
		NEXT_TITLE,
	};
	// メニューシーンのクラス
	class Menu : public SceneState {
	private: // メンバ定数
		const float MAX_EASING_COUNT = 20.0f; // Easingのマックスカウント
		const float SPACE = 70.0f; // 間隔
		const int  EASING_SPACE = 3; // 間隔
		const Math::Vector2 winSize = { 1280.0f,768.0f }; // 画面サイズ
		const Math::Vector2 winCenter = { winSize.x/2,winSize.y / 2 }; // 画面の中央
	public:
		static const int MAX_BUTTON = 5; // スプライトButtonの個数

	private:
		// メニュー内で使う変数宣言の場(Object/Sprite/時間/フラグ等)
		MenuState menuState = MenuState::NONE;
		// ボタン画像
		Sprite* spStageBG1  = nullptr; // 背景
		Sprite* cursor = nullptr; // カーソル
		Sprite* buttons[MAX_BUTTON] = {}; // ボタン
		// 説明画像
		Sprite* playerGuide = nullptr; // 主人公の説明
		Sprite* enemyGuides = nullptr; // 敵の説明
		// その他
		float easingCounts[MAX_BUTTON] = {}; // ボタンの数分のカウンター
		bool menuExists = true; // メニューが開かれてるかどうかの状態
		bool cursorDisp = false; // カーソル表示

	public:
		Menu();
		~Menu();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private:
		void EasingMove(int i); // イージングで移動
		void CountUpdate(int& count); // カウンターを更新
		void CountsUpdate(); // 全てのカウントを更新
	};
}