#include "Story.h"
#include "../GameScene/Select.h"
#include "../GameScene/GameScene.h"
#include "../2D/Sprite.h"
#include "../Input/KeyInput.h"
#include "../Tool/Easing.h"
#include "../Audio/Audio.h"

XIIlib::Story::Story()
{
}

XIIlib::Story::~Story()
{
	for(int i = MAX_STORY -1; i >= 0; --i)
	{
		delete storys[i];
	}
}

void XIIlib::Story::Initialize(GameScene* p_game_scene)
{
	int i = 0;
	storys[i] = Sprite::Create(STORY0_SP, winSize * i); i++;
	storys[i] = Sprite::Create(STORY0_SP, winSize * i); i++;
	storys[i] = Sprite::Create(STORY0_SP, winSize * i); i++;
}

void XIIlib::Story::Update(GameScene* p_game_scene)
{
	Math::Vector2 pos = { 0,0 };
	switch (storyState)
	{
	case XIIlib::Story::StoryState::NOEN:
		// 次のページ
		if (KeyInput::GetInstance()->Trigger(DIK_SPACE))
		{
			if (storys[MAX_STORY - 1]->GetPosition().x <= 0)
			{
				p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
				trigSpace = true;
				return;
			}
			storyState = StoryState::MOVE;
			easingCount = 0;
			for (int i = 0; i < MAX_STORY; ++i)
			{
				defaultPos[i] = storys[i]->GetPosition();
			}
			
		}
		//スキップ
		if (KeyInput::GetInstance()->Trigger(DIK_TAB))
		{
			p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
			trigSpace = true;
		}
		if (trigSpace) {
			if (p_game_scene->DrawScreen(false)) {
				p_game_scene->ChangeState(new Select);
				return;
			}
		}
		break;
	case XIIlib::Story::StoryState::MOVE:
		for (int i = 0; i < MAX_STORY; ++i)
		{
			pos.x = storys[i]->GetPosition().x;
			pos.x = defaultPos[i].x - Easing::InOutCubic(easingCount, 0, winSize.x, MAX_EASING);
			storys[i]->SetPosition(pos);
		}
		if (easingCount >= MAX_EASING) 
		{
			storyState = StoryState::NOEN;
			return;
		}
		easingCount++;
		break;
	default:

		break;
	}
	
}

void XIIlib::Story::Draw()
{
}

void XIIlib::Story::DrawTex()
{
	storys[0]->Draw();
	storys[1]->Draw();
	storys[2]->Draw();
}

void XIIlib::Story::DrawBackground()
{
}
