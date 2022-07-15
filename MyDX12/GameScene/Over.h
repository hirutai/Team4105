#pragma once
#include "SceneState.h"
class Sprite;
namespace XIIlib {

	class GameScene;
	

	// プレイシーンのクラス
	class Over : public SceneState {
	private:
		// プレイ内で使う変数宣言の場(Object/Sprite/時間/フラグ等)

	public:
		Over();
		~Over();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // メンバ変数
		Sprite* spGameOver = nullptr;
		bool oneThrough = false;

		Sprite* space = nullptr;
		Sprite* textRetry = nullptr;
		Sprite* textTitle = nullptr;
		bool selectT_R = false;
	};
}