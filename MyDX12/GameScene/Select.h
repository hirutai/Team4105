#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>

class Sprite;

namespace XIIlib {

	class GameScene;

	// ステージ選択シーンのクラス
	class Select : public SceneState {
	private: // メンバ定数
		const float homePosY = 0;
		const Math::Vector2 HOMEL_POS = {};
		const Math::Vector2 HOMEC_POS = {};
		const Math::Vector2 HOMER_POS = {};
	private: // メンバ変数
		// 変数宣言の場(Object/Sprite/時間/フラグ等)
#pragma region Spriteデータ
		std::unique_ptr<Sprite> easyButton;
		std::unique_ptr<Sprite> normalButton;
		std::unique_ptr<Sprite> hardButton;
		std::unique_ptr<Sprite> edge;
#pragma endregion
		

	public:
		Select();
		~Select();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};
}

