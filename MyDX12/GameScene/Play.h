#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
	class IntervalTimer;

	// プレイシーンのクラス
	class Play : public SceneState {
	private:
		// プレイ内で使う変数宣言の場(Object/Sprite/時間/フラグ等)
		IntervalTimer* intervalTimter = nullptr; // 敵の行動間隔用タイマー
		Sprite* spStageBG1 = nullptr; // 背景
		Sprite* menuButton = nullptr; // メニュー
		Sprite* operatorGuide = nullptr; // 操作説明
		bool menuExists = false; // メニューが開かれてるかどうかの状態
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};
}