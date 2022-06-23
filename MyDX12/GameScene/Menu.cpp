#include "Menu.h"
#include "GameScene.h"
#include "Select.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"
#include "Play.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::Menu::Menu()
{
	// Initializeとほぼ同じ

}

XIIlib::Menu::~Menu()
{
	// ポインタ使ったやつの埋葬場
	delete enemyGuides;
	delete playerGuide;
	for (int i = MAX_BUTTON -1; i >= 0; --i)
	{
		delete buttons[i];
	}
	delete cursor;
	delete spStageBG1;
}

void XIIlib::Menu::Initialize(GameScene* p_game_scene)
{
	spStageBG1   = Sprite::Create(STAGEBG1_TEX,      { 0.0f,0.0f }); // 背景
	cursor       = Sprite::Create(CURSOR_TEX, -winCenter, { 1,1,1,1 }, { 0.5f,0.5f });
	buttons[(int)MenuState::CONTINUE] 
		= Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE * 2}, { 1,1,1,1 }, { 0.5f,0.5f });
	buttons[(int)MenuState::PLAYER_GUIDS] 
		= Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y - SPACE}, { 1,1,1,1 }, { 0.5f,0.5f });
	buttons[(int)MenuState::ENEMY_GUIDS] 
		= Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y },       { 1,1,1,1 }, { 0.5f,0.5f });
	buttons[(int)MenuState::NEXT_SLECT] 
		= Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE}, { 1,1,1,1 }, { 0.5f,0.5f });
	buttons[(int)MenuState::NEXT_TITLE] 
		= Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 2 }, { 1,1,1,1 }, { 0.5f,0.5f });
	// サイズを0に
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}

	playerGuide = Sprite::Create(PLAYERGUIDES_TEX, winSize); // プレイヤーの説明
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, { winSize.x /2 , winSize.y / 2}, { 1,1,1,1 } ,{0.5f,0.5f}); // 敵の説明
}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
	// 最後のカウンターがMAXに到達するまで、Easing処理
	if (easingCounts[MAX_BUTTON -1] < MAX_EASING_COUNT)
	{
		for (int i = 0; i < MAX_BUTTON; ++i)
		{
			EasingMove(i);
		}
		CountsUpdate();
	}
	else
	{
		// 更新
		if (KeyInput::GetInstance()->Trigger(DIK_TAB)) {
			p_game_scene->ChangeState(new Play);
			menuExists = false;
		}
	}
	
	// もしkeyを押したら
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S))
	{
		// それぞれの処理
		if (KeyInput::GetInstance()->Trigger(DIK_W))
		{

		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S))
		{

		}
	}

	// テスト用
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}

}

void XIIlib::Menu::Draw()
{
	// 3D描画
	AttackAreaManager::GetInstance()->Draw();
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Menu::DrawTex()
{
	// スプライト描画
	cursor->Draw();
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->Draw();
	}
	//playerGuide->Draw();
	//enemyGuides->Draw();
}

void XIIlib::Menu::DrawBackground()
{
	spStageBG1->Draw();
}

void XIIlib::Menu::EasingMove(int i)
{
	// countがマックスに到達するまで
	if (easingCounts[i] > MAX_EASING_COUNT) return;

	Math::Vector2 size = { 0,0 };
	size.x = Easing::InOutCubic(easingCounts[i], 0, 94, MAX_EASING_COUNT);
	size.y = Easing::InOutCubic(easingCounts[i], 0, 34, MAX_EASING_COUNT);
	buttons[i]->SetSize(size);
}

void XIIlib::Menu::CountsUpdate()
{
	// 全てのカウントインクリメント
	int count = 0;
	CountUpdate(count);
}

void XIIlib::Menu::CountUpdate(int& count)
{
	// カウントをインクリメント、遅延分のカウントより大きくなったら次に行く
	easingCounts[count]++;
	if (easingCounts[count] < EASING_SPACE)return;
	count++;
	if (count >= MAX_BUTTON)return;
	CountUpdate(count);
}

