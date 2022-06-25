#include "Story.h"
#include "../GameScene/Select.h"
#include "../GameScene/GameScene.h"
#include "../2D/Sprite.h"
#include "../Input/KeyInput.h"

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
	storys[i] = Sprite::Create(STORY1_SP, winSize * i); i++;
	storys[i] = Sprite::Create(STORY2_SP, winSize * i); i++;
}

void XIIlib::Story::Update(GameScene* p_game_scene)
{
	// 次のページ
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE))
	{

	}

	//スキップ
	if (KeyInput::GetInstance()->Trigger(DIK_TAB))
	{
		p_game_scene->ChangeState(new Select);
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
