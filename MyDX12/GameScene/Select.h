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
		const float GAP = 100.0f;
		const float HOME_POSY = 250;
		const Math::Vector2 BASE_SIZE = {360,240};
		const Math::Vector2 HOMEL_POS 
			= {0,HOME_POSY };
		const Math::Vector2 HOMEC_POS 
			= {HOMEL_POS.x + BASE_SIZE.x + GAP ,HOME_POSY };
		const Math::Vector2 HOMER_POS 
			= { HOMEC_POS.x + BASE_SIZE.x + GAP ,HOME_POSY };
	private: // メンバ変数
		// 変数宣言の場(Object/Sprite/時間/フラグ等)
		Math::Vector2 edgePos = HOMEL_POS;
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

