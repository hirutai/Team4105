#pragma once
#include <vector>
#include "../Input/GamePAD_XInput.h"
#include "../Struct/Math.h"
class Sprite;

class DebugCamera;

namespace XIIlib
{
	class Audio;
	class SceneState;
	class Curtain;
	
	enum struct TransitionType : int {
		OPEN,
		CLOSE,
		WHITE,
		BLACK
	};

	class GameScene
	{
	public:
		GameScene();
		~GameScene();
		// シーンの切り替え
		void ChangeState(SceneState* different_state);

		// 基本関数(RegularFunction)
		void Initialize();
		void Update();
		void Draw();
		void DrawSprite();
		void DrawBackground();

		bool DrawScreen(const TransitionType& tType);

		bool OpenedCurtain();
		bool ClosedCurtain();

		bool WhiteOut();
		bool BlackOut();
		// 関数配列用の定義
		typedef bool (XIIlib::GameScene::*FLAGFUNC)();

		//void SetGamePad();

		// カメラの設定
		void SetDebugCamera(DebugCamera* debugCamera_) { debugCamera = debugCamera_; }
		// カメラの取得
		DebugCamera* GetCamera() { return debugCamera; }

		Audio* GetAudio()const;

		// a値のリセット
		void ResetAlpha();
	private:
		// 状態
		SceneState* state = nullptr;
		// 音
		Audio* audio = nullptr;
		// カメラ
		DebugCamera* debugCamera = nullptr;
		// GamePad
		GamePAD_XInput* gamePad = nullptr;

		// 制御
		int isTurn = 0;

		// 攻撃用フレームカウント
		int attackFrame = 0;

		// マスポインタ
		int squareX = 0;
		int squareZ = 0;

		int _timer = 0;
		int at_type = 0;

		std::vector<Curtain*> curtain;

		// 返り値がbool型の関数のvectorコンテナ
		std::vector<FLAGFUNC> functions;

		// ホワイトアウト、ブラックアウト演出関連
		Sprite* outTex = nullptr; // 白画像

		const Math::Vector3 white{ 1.0f, 1.0f, 1.0f }; // 白
		const Math::Vector3 black{ 0.0f, 0.0f,0.0f }; // 黒

		const float incValue = 0.01f; // a値の増加量
		const float maxAlpha = 1.0f; // a値の最大値

		Math::Vector3 outColor; // 色保存用
		float outAlpha = 0.0f; // α値保存用
	};
}