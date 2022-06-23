#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;

	enum class Timing
	{
		CameraDirecting, // カメラ演出
		Game, // ゲーム
	};

	// プレイシーンのクラス
	class Play : public SceneState {
	private:
		// プレイ内で使う変数宣言の場(Object/Sprite/時間/フラグ等)
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

#pragma region カメラ演出関連
	private: // メンバ変数
		Timing timing = Timing::CameraDirecting; // 現在のタイミング

		const float finalEye = 18.0f; // 演出後の視点

		Math::Vector3 cameraEye{}; // カメラの視点
		Math::Vector3 cameraTarget{}; // カメラ注視点

		float cameraRotation = -30.0f; // 回転量
#pragma endregion
	};
}