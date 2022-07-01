#pragma once
#include <vector>
#include "../Input/GamePAD_XInput.h"
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
	};
}