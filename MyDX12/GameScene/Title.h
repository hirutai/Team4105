#pragma once
#include "SceneState.h"

class Sprite;

namespace XIIlib {

	class GameScene;

	// タイトルシーンのクラス
	class Title : public SceneState {
	private:
		// タイトル内で使う変数宣言の場(Object/Sprite/時間/フラグ等)

	public:
		Title();
		~Title();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private:
		Sprite* spTitleBG = nullptr;
		Sprite* spTitleLog = nullptr;

		Sprite* space = nullptr;
	};
}