#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib
{
	class GameScene;
	class Story : public SceneState
	{
	private:
		static const int MAX_STORY = 3;
		const Math::Vector2 winSize = {1280.0f,768.0f};
	private:
		Sprite* storys[MAX_STORY] = {};
	public:
		Story();
		~Story();

		// 初期化
		void Initialize(GameScene* p_game_scene)override;
		// 更新
		void Update(GameScene* p_game_scene)override;
		// 描画
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};

}

