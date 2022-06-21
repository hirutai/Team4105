#include "Play.h"
#include "GameScene.h"
#include "Menu.h"
#include "Clear.h"
#include "Over.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../GameObject/Yankee.h"
#include "../GameObject/Stone.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../GameObject/IntervalTimer.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../Tool/Easing.h"

XIIlib::Play::Play()
{
	// Initializeとほぼ同じ

}

XIIlib::Play::~Play()
{
	// ポインタ使ったやつの埋葬場
	delete enemyGuides; // 敵の説明
	delete operatorGuide; // 操作説明
	delete menu; // メニュー
	delete spStageBG1;
	delete intervalTimter;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	
	// IntervalTimer newと初期化
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(4, 5);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);
	
	if (stageNum == StageNumber::EASY)
	{
		// 背景画像生成
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::NORMAL)
	{
		// 背景画像生成
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::HARD)
	{
		// 背景画像生成
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}

	operatorGuide = Sprite::Create(PLAYERGUIDE_TEX, { 1000.0f,600.0f }); // 操作説明
	menu = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // メニュー
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, {0.0f,0.0f});; // 敵の説明
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
#pragma region メニュー処理

	// メニュー画面を展開、閉じる
	if (KeyInput::GetInstance()->Trigger(DIK_TAB))
	{
		p_game_scene->ChangeState(new Menu); // クリアシーンへ
		if (menuExists && easingCount >= MAX_EASING_COUNT)
		{
			// ゼロClear
			easingCount = 0;
			menuExists = false;
		}
		else
		{
			menuExists = true;
		}
	}

	if (menuExists)
	{
		float posX = 0;
		float posY = 0;
		// countがマックスに到達するまで
		if (easingCount <= MAX_EASING_COUNT)
		{
			posX = Easing::EaseInOutElastic(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
			posY = Easing::EaseInOutElastic(easingCount, -winSize.y, winSize.y, MAX_EASING_COUNT);
			easingCount++;
		}
		enemyGuides->SetPosition({ posX,posY });
	}

	// メニューが展開されているならreturn
	if (menuExists)return;
#pragma endregion 

#pragma region Game Update処理
	// 更新
	UnitManager::GetInstance()->Update();
	intervalTimter->Timer();
	// シーン移動
	if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // プレイヤが存在している場合
	{
		if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // 敵を全滅させた時
		{
			trigSpace = true;
		}
	}
	else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // プレイヤが存在していない場合
	{
		trigSpace = true;
	}
#pragma endregion
	if (trigSpace) {
		if (p_game_scene->DrawScreen(false)) {
			if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // プレイヤが存在している場合
			{
				if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // 敵を全滅させた時
				{
					p_game_scene->ChangeState(new Clear); // クリアシーンへ
				}
			}
			else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // プレイヤが存在していない場合
			{
				p_game_scene->ChangeState(new Over); // オーバーシーンへ
			}
		}
	}
}

void XIIlib::Play::Draw()
{
	AttackAreaManager::GetInstance()->Draw();
	// 3D描画
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// スプライト描画
	//intervalTimter->Draw();
	operatorGuide->Draw();
	menu->Draw();
	// メニューが展開されていないならreturn
	if (!menuExists)return;
	enemyGuides->Draw();
	menu->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// 背景スプライト描画
	spStageBG1->Draw();
}
