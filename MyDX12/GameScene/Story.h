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

		// èâä˙âª
		void Initialize(GameScene* p_game_scene)override;
		// çXêV
		void Update(GameScene* p_game_scene)override;
		// ï`âÊ
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};

}

