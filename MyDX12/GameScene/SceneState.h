#pragma once

namespace XIIlib
{
	class GameScene;

	// 基底状態クラス
	class SceneState
	{
	protected:
		SceneState(){}
	public:
		// コピー禁止
		SceneState(const SceneState&) = delete;
		SceneState& operator=(const SceneState&) = delete;

		// 仮想デストラクタ
		virtual ~SceneState() = default;

	public:
		virtual void Initialize(GameScene* p_game_scene) = 0;// 初期化
		virtual void Update(GameScene* p_game_scene) = 0;//　更新
		virtual void Draw() = 0;// 描画
		virtual void DrawTex() = 0;//スプライト描画
	};
}