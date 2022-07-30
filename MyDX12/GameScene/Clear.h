#pragma once
#include "SceneState.h"
class Sprite;

/// <summary>
/// ゲームクリアシーン
/// </summary>
namespace XIIlib
{
	class GameScene;
	class ChainSprite;

	class Clear : public SceneState
	{
	private:
		const float yMullValue1 = 0.5f, yMullValue2 = 0.7f,
			xMullValue = 0.53f, mulXY = 1.5f, pickSize = 1.5f;
		const float xMullValueHard = 0.3f;
	public: // メンバ関数
		// コンストラクタ
		Clear();
		// デスストラクタ
		~Clear();

		// 初期化
		void Initialize()override;
		// 更新
		void Update()override;
		// 描画
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // メンバ変数
		Sprite* gameClear = nullptr; // クリア画像
		bool oneThrough = false;

		Sprite* textSelect = nullptr;
		Sprite* textTitle = nullptr;
		ChainSprite* pchStr = nullptr;
		bool selectT_R = false;
	};

}