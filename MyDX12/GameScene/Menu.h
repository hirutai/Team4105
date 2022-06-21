#pragma once
#include "SceneState.h"

namespace XIIlib {

	class GameScene;

	// メニューシーンのクラス
	class Menu : public SceneState {
	private:
		// メニュー内で使う変数宣言の場(Object/Sprite/時間/フラグ等)

	public:
		Menu();
		~Menu();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private:

	};
}