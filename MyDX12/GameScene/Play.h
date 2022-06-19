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

	private: // メンバ定数
<<<<<<< HEAD
		const float MAX_EASING_COUNT = 30.0f;
=======
		const float MAX_EASING_COUNT = 60.0f;
>>>>>>> 4965877ae8c2e767f0ad462363b71c4f4a74aeb4
		const Math::Vector2 winSize = {1280.0f,768.0f};
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // メンバ変数
		Sprite* spStageBG1 = nullptr; // 背景
		Sprite* playerGuide = nullptr; // 操作説明
		Sprite* menu = nullptr; // メニュー
		Sprite* enemyGuides = nullptr; // 敵の説明
		bool menuExists = false;
<<<<<<< HEAD
		bool exitFlag = false;
=======
>>>>>>> 4965877ae8c2e767f0ad462363b71c4f4a74aeb4
		//Math::Vector2 eGuidesPos = {1280.0f,0.0f};
		float easingCount = 0;
	};
}