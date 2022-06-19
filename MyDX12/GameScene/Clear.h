#pragma once
#include "SceneState.h"
class Sprite;

/// <summary>
/// ゲームクリアシーン
/// </summary>
namespace XIIlib
{
	class GameScene;

	class Clear : public SceneState
	{
	public: // メンバ関数
		// コンストラクタ
		Clear();
		// デスストラクタ
		~Clear();

		// 初期化
		void Initialize(GameScene* p_game_scene)override;
		// 更新
		void Update(GameScene* p_game_scene)override;
		// 描画
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // メンバ変数
		Sprite* gameClear = nullptr; // クリア画像
		bool oneThrough = false;
	};

}