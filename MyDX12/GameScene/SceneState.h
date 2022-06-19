#pragma once
#include "../Input/GamePAD_XInput.h"
namespace XIIlib
{
	class GameScene;

	// スプライトの名前
	enum class SpriteName : int
	{
		PUSHA_SP = 6,
		GAMEOVER_SP,
		STAGEBG1_SP,
		EASY_BUTTON_SP,
		NORMAL_BUTTON_SP,
		HARD_BUTTON_SP,
		EDGE_SP,
		TITLEBG_SP,
		TITLELOG_SP,
		GAMECLEAR_SP,
		PLAYERGUIDE_SP,
		MENU_SP,
		ENEMYGUIDES_SP,
	};

	// ステージナンバー
	enum class StageNumber : int
	{
		DEBUG = -1,
		EASY,
		NORMAL,
		HARD,
	};

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
		virtual void DrawBackground() = 0;
	public: // Setter関数
		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; } // GamePadの設定
	protected: // 静的メンバ変数
		static StageNumber stageNum; // ステージナンバー
		// GamePadの借り物変数
		static GamePAD_XInput* gamePad_;
	};
}