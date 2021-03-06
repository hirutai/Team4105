#include "Menu.h"
#include "GameScene.h"
#include "Select.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"
#include "Play.h"
#include "Select.h"
#include "Title.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"
#include "../3D/Object3D.h"

XIIlib::Menu::Menu()
{
	// Initializeとほぼ同じ

}

XIIlib::Menu::~Menu()
{
	// ポインタ使ったやつの埋葬場
}

void XIIlib::Menu::Initialize()
{
	// 定数
	const DirectX::XMFLOAT4 color = {1,1,1,1};
	const Math::Vector2 anchorPoint = {0.5f,0.5f};

	// 生成
	spStageBG1.reset(Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f })); // 背景
	cursor.reset(Sprite::Create(CURSOR_TEX, { winCenter.x - CURSOR_SPACE, winCenter.y - SPACE }, color, anchorPoint));
	playerGuide.reset(Sprite::Create(PLAYERGUIDES_TEX, winCenter, color, anchorPoint)); // プレイヤーの説明
	playerGuide->SetSize(winCenter);
	enemyGuides.reset(Sprite::Create(ENEMYGUIDES_TEX, winCenter, color, anchorPoint)); // 敵の説明
	enemyGuides->SetSize(winCenter);

	rulePos = { winCenter.x, winCenter.y - SPACE * 2.5f };
	if (stageNum == StageNumber::HARD)
	{
		buttons[(int)CursorState::RULE]
			.reset(Sprite::Create(BOSSCLEARCONDITION,rulePos, color, anchorPoint));
	}
	else
	{
		buttons[(int)CursorState::RULE]
			.reset(Sprite::Create(CLEARCONDITION,rulePos, color, anchorPoint));
	}
	buttons[(int)CursorState::CONTINUE] 
		.reset(Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE}, color, anchorPoint));
	buttons[(int)CursorState::PLAYER_GUIDS] 
		.reset(Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y}, color, anchorPoint));
	buttons[(int)CursorState::ENEMY_GUIDS] 
		.reset(Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y + SPACE }, color, anchorPoint));
	buttons[(int)CursorState::NEXT_SLECT] 
		.reset(Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE * 2 }, color, anchorPoint));
	buttons[(int)CursorState::NEXT_TITLE] 
		.reset(Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 3 }, color, anchorPoint));

	//for (int i = 0; i < MAX_BUTTON; i++)
	//{
	//	buttons[i]->SetSize({0.5f,0.5f});
	//}

	// 最大最小値を決める
	CheckLimitPos();


	// 全てのサイズを0に
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}
}

void XIIlib::Menu::Update()
{
#pragma region Easing処理
	// Easing処理
	EasingUpdate();
	// イージング処理中なら即リターン
	if (easingState != EasingState::NONE)return;
#pragma endregion

#pragma region 説明
	// 勝利条件移動処理
	static Math::Vector3 move;
	if (-600.0f >= rulePos.x)rulePos.x = (winCenter.x * 2) + 300.0f;
	if (count >= moveTime) {
		rulePos.x += VEL_X;
		buttons[0]->SetPosition(rulePos);
	}
	else
	{
		// 震える処理
		if (count >= 120) {
			switchRand.x = switchRandiMin + (int)(rand() * (switchRandiMax - switchRandiMin + 1) / (1 + RAND_MAX));
			switchRand.y = switchRandiMin + (int)(rand() * (switchRandiMax - switchRandiMin + 1) / (1 + RAND_MAX));
			buttons[0]->SetPosition({ rulePos.x + switchRand.x,rulePos.y + switchRand.y });
		}
	}
	count++;

	// menu状態が説明表示になっていたら
	if (menuState == MenuState::PLAYER_GUIDS || menuState == MenuState::ENEMY_GUIDS)
	{
		if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || KeyInput::GetInstance()->Trigger(DIK_TAB) 
			|| gamePad_->Button_Down(X_A) || gamePad_->Button_Down(X_START))
		{
			menuState = MenuState::NONE;
		}
		return;
	}
#pragma endregion

#pragma region メニュー処理
	// カーソルの表示
	if (!cursorDisp) { cursorDisp = true; }
	
	// カーソル移動
	MoveCursor();

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A))
	{
		menuState = (MenuState)cursorState;
		if (menuState == MenuState::PLAYER_GUIDS)
		{
			playerGuide->SetPosition(winCenter);
		}
		else if (menuState == MenuState::ENEMY_GUIDS)
		{
			enemyGuides->SetPosition(winCenter);
		}
		// シーン移動関連なら
		else if (cursorState == CursorState::CONTINUE ||
			cursorState == CursorState::NEXT_SLECT ||
			cursorState == CursorState::NEXT_TITLE)
		{
			easingState = EasingState::MOVE_OUT;
		}
		return;
	}

	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_TAB) || gamePad_->Button_Down(X_START)) {
		easingState = EasingState::MOVE_OUT;
	}
#pragma endregion 
}

void XIIlib::Menu::Draw()
{
	SceneState::BackStagesDraw();
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

	if (menuState == MenuState::PLAYER_GUIDS)
	{
		playerGuide->Draw();
	}
	else if (menuState == MenuState::ENEMY_GUIDS)
	{
		enemyGuides->Draw();
	}

}

void XIIlib::Menu::DrawBackground()
{
	spStageBG1->Draw();
}

void XIIlib::Menu::EasingUpdate()
{
	// 最後のカウンターがMAXに到達するまで、Easing処理
	if (easingState == EasingState::MOVE_IN || easingState == EasingState::MOVE_OUT)
	{
		// カーソルの非表示
		if (cursorDisp) { cursorDisp = false; }
		// Easing処理
		for (int i = 0; i < MAX_BUTTON; ++i)
		{
			EasingMove(i, easingState);
		}
		// カウントの更新
		CountsUpdate();

		// 最後のカウントがマックスになったらEasingの状態をNONE
		if (easingCounts[MAX_BUTTON - 1] > MAX_EASING_COUNT)
		{
			// EasingOutなら別シーンに移動
			if (easingState == EasingState::MOVE_OUT)
			{
				switch (menuState)
				{
				case XIIlib::MenuState::CONTINUE:
					p_game_scene->ChangeState(new Play);
					break;
				case XIIlib::MenuState::NEXT_SLECT:
					// シーンを戻る際はUnitデータを消しておく
					UnitManager::GetInstance()->AllDestroy();
					p_game_scene->ChangeState(new Select);
					break;
				case XIIlib::MenuState::NEXT_TITLE:
					// シーンを戻る際はUnitデータを消しておく
					UnitManager::GetInstance()->AllDestroy();
					p_game_scene->ChangeState(new Title);
					break;
				default:
					p_game_scene->ChangeState(new Play);
					break;
				}

				menuExists = false;
				return;
			}
			// カウントをすべて0
			for (int i = 0; i < MAX_BUTTON; ++i)
			{
				easingCounts[i] = 0;
			}
			// 状態をNONEに
			easingState = EasingState::NONE;
		}
	}
}

void XIIlib::Menu::EasingMove(int i,EasingState easingState)
{
	// countがマックスに到達するまで
	if (easingCounts[i] > MAX_EASING_COUNT) return;
	const Math::Vector2 defaultSize = buttons[i]->GetDefault();
	Math::Vector2 size = { 0,0 };
	float alpha = 0;
	size.x = Easing::InOutCubic(easingCounts[i], 0, defaultSize.x, MAX_EASING_COUNT);
	size.y = Easing::InOutCubic(easingCounts[i], 0, defaultSize.y, MAX_EASING_COUNT);
	alpha = Easing::InOutCubic(easingCounts[i], 0, 1, MAX_EASING_COUNT);
	if (easingState == EasingState::MOVE_IN)
	{
		// 0 + ...なのでそのまま
		if(i == 0)buttons[i]->SetSize(size * 1.5f);
		else buttons[i]->SetSize(size);
		buttons[i]->SetAlpha(alpha);
	}
	else if (easingState == EasingState::MOVE_OUT)
	{
		// その分引く
		if (i == 0)buttons[i]->SetSize((defaultSize * 1.5f) - size);
		else buttons[i]->SetSize(defaultSize - size);
		buttons[i]->SetAlpha(1 - alpha);
	}
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

void XIIlib::Menu::CheckLimitPos()
{
	// 最大と最小を調べる
	float min = buttons[1]->GetPosition().y;
	float max = buttons[1]->GetPosition().y;
	for (int i = 1; i < MAX_BUTTON; ++i)
	{
		if (min > buttons[i]->GetPosition().y)
		{
			min = buttons[i]->GetPosition().y;
		}

		if (max < buttons[i]->GetPosition().y)
		{
			max = buttons[i]->GetPosition().y;
		}
	}
	// 代入
	minPosY = min;
	maxPosY = max;
}

void XIIlib::Menu::MoveCursor()
{
	prevPos = cursor->GetPosition();
	// もしkeyを押したら
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S)
		|| gamePad_->LStickDownFront() || gamePad_->LStickDownBack())
	{
		// 移動処理
		float moveY = 0;
		Math::Vector2 pos = cursor->GetPosition();
		// それぞれの処理
		if (KeyInput::GetInstance()->Trigger(DIK_W) || gamePad_->LStickDownFront())
		{
			moveY = -SPACE;
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S) || gamePad_->LStickDownBack())
		{
			moveY = SPACE;
		}
		pos.y += moveY;
		// 枠を超えたら止める
		if (pos.y < minPosY)
		{
			pos.y = minPosY;
		}
		else if (pos.y > maxPosY)
		{
			pos.y = maxPosY;
		}
		// 座標を設定
		cursor->SetPosition(pos);
	}
	// メニューの情報
	CheckMenuState();
}

void XIIlib::Menu::CheckMenuState()
{
	// 近似値
	float nearValue = 5;
	float posY = cursor->GetPosition().y;
	// 前回と同じなら即リターン
	if (posY == prevPos.y)return;
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		if (posY <= buttons[i]->GetPosition().y + nearValue && posY >= buttons[i]->GetPosition().y - nearValue)
		{
			cursorState = (CursorState)i;
			return;
		}
	}
}

