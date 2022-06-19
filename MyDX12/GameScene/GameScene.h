#pragma once
#include <vector>
#include "../Input/GamePAD_XInput.h"
class Sprite;

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
		void DrawBackground();

		bool DrawScreen(bool on_curtain);
		//void SetGamePad();

		Audio* GetAudio()const;

	private:
		// 状態
		SceneState* state = nullptr;
		// 音
		Audio* audio = nullptr;
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

		std::vector<std::vector<unsigned int>> m_curtain;
		std::vector<Sprite*> screen_curtain;
		std::vector<float> time_curtain;

	};
}