#pragma once
#include "../Input/GamePAD_XInput.h"
#include "../2D/SpriteLoader.h"
#include <string>
#include <memory>
class DebugCamera;
class Object3D;

namespace XIIlib
{
	class GameScene;

	enum class Phase
	{
		CameraDirecting, // カメラ演出
		ClearCondDisplay, // クリア条件表示
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
		DebugCamera* debugCamera = nullptr; // カメラ

	protected:
		SceneState(){}
	public:
		// コピー禁止
		SceneState(const SceneState&) = delete;
		SceneState& operator=(const SceneState&) = delete;

		// 仮想デストラクタ
		virtual ~SceneState() = default;

	public: // 継承
		virtual void Initialize() = 0;// 初期化
		virtual void Update() = 0;//　更新
		virtual void Draw() = 0;// 描画
		virtual void DrawTex() = 0;//スプライト描画
		virtual void DrawBackground() = 0;
		void CommonUpdate(GameScene* p_game_scene);
	protected: // 共通処理
		void CreateUnitsPosition(StageNumber stageNum,std::string fileName);
	public: // Setter関数
		void SetGameScene(GameScene* p_game_scene) { this->p_game_scene = p_game_scene; }
		// GamePadの設定
		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; } 
		// カメラの設定
		void SetDebugCamera(DebugCamera* debugCamera_) { debugCamera = debugCamera_; }

		static Phase GetPhase() { return phase; }

	protected:
		// 背景オブジェクト
		static void BackStagesInit();
		static void BackStagesDraw();

	protected: // 静的メンバ変数
		static StageNumber stageNum; // ステージナンバー
		static GameScene* p_game_scene;
		// GamePadの借り物変数
		static GamePAD_XInput* gamePad_;
		static Phase phase; // 現在のフェーズ
		static std::unique_ptr<Object3D> backStage;
		static std::unique_ptr<Object3D> bossBG[]; // ボス背景
		static int easyCount;
		static int normalCount;
	};
}