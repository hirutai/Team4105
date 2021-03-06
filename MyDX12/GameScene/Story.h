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
		enum class StoryState
		{
			NOEN,
			MOVE,
		};
		StoryState storyState = StoryState::NOEN;
	private:
		static const int MAX_STORY = 3;
		const Math::Vector2 winSize = {1280.0f,768.0f};
		const int MAX_EASING = 20;
	private:
		Sprite* storys[MAX_STORY] = {};
		Math::Vector2 defaultPos[MAX_STORY] = {};
		int easingCount = 0;
	public:
		Story();
		~Story();

		// 初期化
		void Initialize()override;
		// 更新
		void Update()override;
		// 描画
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};

}

