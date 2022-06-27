#pragma once
#include "../Input/GamePAD_XInput.h"
#include "../2D/SpriteLoader.h"

class DebugCamera;

namespace XIIlib
{
	class GameScene;

	enum class Phase
	{
		CameraDirecting, // カメラ演出
		Game, // ゲーム
	};

	// ステージナンバー
	enum class StageNumber : int
	{
		DEBUG = -1,
		NONE,
		EASY,
		NORMAL,
		HARD,
	};

	// 基底状態クラス
	class SceneState
	{
	protected:// メンバ変数
		bool trigSpace = false;
		bool movingScene = false;

	protected:
		SceneState(){}
	public:
		// コピー禁止
		SceneState(const SceneState&) = delete;
		SceneState& operator=(const SceneState&) = delete;

		// 仮想デストラクタ
		virtual ~SceneState() = default;

	public: // 継承
		virtual void Initialize(GameScene* p_game_scene) = 0;// 初期化
		virtual void Update(GameScene* p_game_scene) = 0;//　更新
		virtual void Draw() = 0;// 描画
		virtual void DrawTex() = 0;//スプライト描画
		virtual void DrawBackground() = 0;
		void CommonUpdate(GameScene* p_game_scene);
	protected: // 共通処理
		void CreateUnitsPosition(StageNumber stageNum);
	public: // Setter関数
		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; } // GamePadの設定

		// カメラの設定
		void SetDebugCamera(DebugCamera* debugCamera_) { debugCamera = debugCamera_; }
	protected: // 静的メンバ変数
		static StageNumber stageNum; // ステージナンバー
		// GamePadの借り物変数
		static GamePAD_XInput* gamePad_;
		static Phase phase; // 現在のフェーズ

		DebugCamera* debugCamera = nullptr; // カメラ
	};
}