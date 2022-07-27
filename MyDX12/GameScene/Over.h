#pragma once
#include "SceneState.h"
class Sprite;
namespace XIIlib {

	class GameScene;
	

	// プレイシーンのクラス
	class Over : public SceneState {
	private:
		// プレイ内で使う変数宣言の場(Object/Sprite/時間/フラグ等)

	private:
		const float yMullValue1 = 0.5f, yMullValue2 = 0.7f,
			xMullValue = 0.53f, mulXY = 1.5f, pickSize = 1.5f;
	public:
		Over();
		~Over();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // メンバ変数
		Sprite* spGameOver = nullptr;
		bool oneThrough = false;

		Sprite* textRetry = nullptr;
		Sprite* textTitle = nullptr;
		bool selectT_R = false;
	};
}