#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>
class Sprite;
class Object3D;

namespace XIIlib {

	class GameScene;

	enum class Timing
	{
		ToTheRight, // 右へ
		ToTheBack, // 後ろへ
		ToTheLeft, // 左へ
		ToTheFront, // 前へ
		ToTheUp, // 上へ
	};

	// プレイシーンのクラス
	class Play : public SceneState {
	private:
		// プレイ内で使う変数宣言の場(Object/Sprite/時間/フラグ等)
		Sprite* spStageBG1 = nullptr; // 背景
		Sprite* menuButton = nullptr; // メニュー
		Sprite* operatorGuide = nullptr; // 操作説明
		bool menuExists = false; // メニューが開かれてるかどうかの状態
		Object3D* backStage = nullptr;
		int count = 0;
		//std::unique_ptr<Object3D> backStage;
		//std::unique_ptr<Object3D> backStage;
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
		const Math::Vector3 rightEye{ 30,10,0 }; // 右側カメラ座標
		const Math::Vector3 backEye{ 0,10,30 }; // 後ろ側カメラ座標
		const Math::Vector3 leftEye{ -30,10,0 }; // 左側カメラ座標
		const Math::Vector3 frontEye{ 0,10,-30 }; // 前側カメラ座標
		const Math::Vector3 upperEye{ 0,45, -30 }; // 上側カメラ座標

		const float moveValue = 0.5f;

		float x = 0;
		float y = 0;
		float z = 0;

		Math::Vector3 cameraEye{ frontEye.x, frontEye.y, frontEye.z }; // カメラの視点

		Timing timing = Timing::ToTheRight; // 現在のタイミング

		void ToTheRight(); // 右へ

		void ToTheBack(); // 後ろへ

		void ToTheLeft(); // 左へ

		void ToTheFront(); // 前へ

		void ToTheUp(); // 上へ
#pragma endregion

		bool cPushFlag = false;
		bool oPushFlag = false;
	};
}