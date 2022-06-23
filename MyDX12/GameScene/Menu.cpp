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
	// 定数
	const DirectX::XMFLOAT4 color = {1,1,1,1};
	const Math::Vector2 anchorPoint = {0.5f,0.5f};

	// 生成
	spStageBG1   = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f }); // 背景
	cursor       = Sprite::Create(CURSOR_TEX, { winCenter.x - 80.0f, winCenter.y - SPACE * 2 }, color, anchorPoint);

	buttons[(int)MenuState::CONTINUE] 
		= Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE * 2}, color, anchorPoint);
	buttons[(int)MenuState::PLAYER_GUIDS] 
		= Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y - SPACE}, color, anchorPoint);
	buttons[(int)MenuState::ENEMY_GUIDS] 
		= Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y }, color, anchorPoint);
	buttons[(int)MenuState::NEXT_SLECT] 
		= Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE}, color, anchorPoint);
	buttons[(int)MenuState::NEXT_TITLE] 
		= Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 2 }, color, anchorPoint);

	// サイズを0に
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}

	playerGuide = Sprite::Create(PLAYERGUIDES_TEX, winCenter, color, anchorPoint); // プレイヤーの説明
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, winCenter, color, anchorPoint); // 敵の説明
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
		// カーソルの表示
		if (!cursorDisp) { cursorDisp = true; }
		// 更新
		if (KeyInput::GetInstance()->Trigger(DIK_TAB)) {
			p_game_scene->ChangeState(new Play);
			menuExists = false;
		}
	}
	
	// もしkeyを押したら
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S))
	{
		Math::Vector2 move = {0.0f,0.0f};
		Math::Vector2 pos = cursor->GetPosition();
		// それぞれの処理
		if (KeyInput::GetInstance()->Trigger(DIK_W))
		{
			move.y = -SPACE;
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S))
		{
			move.y = SPACE;
		}
		pos.y += move.y;
		// 枠を超えたら止める
		if (pos.y < buttons[0]->GetPosition().y) 
		{
			pos.y = buttons[0]->GetPosition().y;
		}
		else if (pos.y > buttons[MAX_BUTTON - 1]->GetPosition().y)
		{
			pos.y = buttons[MAX_BUTTON - 1]->GetPosition().y;
		}

		cursor->SetPosition(pos);
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
	if (cursorDisp) {
		cursor->Draw();
	}
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
	float alpha = 0;
	size.x = Easing::EaseInOutCubic(easingCounts[i], 0, 94, MAX_EASING_COUNT);
	size.y = Easing::EaseInOutCubic(easingCounts[i], 0, 34, MAX_EASING_COUNT);
	alpha = Easing::EaseInOutCubic(easingCounts[i], 0, 1, MAX_EASING_COUNT);
	buttons[i]->SetSize(size);
	buttons[i]->SetAlpha(alpha);
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

