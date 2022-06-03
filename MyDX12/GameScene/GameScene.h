#pragma once
#include <vector>

namespace XIIlib
{
	class Audio;
	class SceneState;

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

		Audio* GetAudio()const;

	private:
		// 状態
		SceneState* state = nullptr;
		// 音
		Audio* audio = nullptr;
		
		// 制御
		int isTurn = 0;

		// 攻撃用フレームカウント
		int attackFrame = 0;

		// マスポインタ
		int squareX = 0;
		int squareZ = 0;

		int _timer = 0;
		int at_type = 0;

		//うんち
	};
}