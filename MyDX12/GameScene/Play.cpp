#include "Play.h"
#include "GameScene.h"
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
		// Scene切り替え時に一度通る処理
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));

		// 背景画像生成
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::NORMAL)
	{
		// Scene切り替え時に一度通る処理
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		//std::shared_ptr<Knight> knight = std::move(Knight::Create(7, 7));
		//std::shared_ptr<Knight> knight2 = std::move(Knight::Create(6, 2));
		//std::shared_ptr<Knight> knight3 = std::move(Knight::Create(7, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		//std::shared_ptr<Bishop> bishop2 = std::move(Bishop::Create(3, 5));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(3, 6));
		//std::shared_ptr<Rook> rook2 = std::move(Rook::Create(6, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 3));
		std::shared_ptr<Yankee> yankee2 = std::move(Yankee::Create(5, 1));
		std::shared_ptr<Yankee> yankee3 = std::move(Yankee::Create(5, 6));
		std::shared_ptr<Yankee> yankee4 = std::move(Yankee::Create(5, 7));
		//std::shared_ptr<Stone> stone = std::move(Stone::Create(6, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		//UnitManager::GetInstance()->AddUnit(std::move(knight));
		//UnitManager::GetInstance()->AddUnit(std::move(knight2));
		//UnitManager::GetInstance()->AddUnit(std::move(knight3));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		//UnitManager::GetInstance()->AddUnit(std::move(bishop2));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		//UnitManager::GetInstance()->AddUnit(std::move(rook2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));
		UnitManager::GetInstance()->AddUnit(std::move(yankee2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee3));
		UnitManager::GetInstance()->AddUnit(std::move(yankee4));
		//UnitManager::GetInstance()->AddUnit(std::move(stone));

		// 背景画像生成
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}

	operatorGuide = Sprite::Create(PLAYERGUIDE_TEX, { 1000.0f,600.0f }); // 操作説明
	menu = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // メニュー
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, {0.0f,0.0f});; // 敵の説明
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
	UnitManager::GetInstance()->Update();
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
#pragma region メニュー処理

	// メニュー画面を展開、閉じる
	if (KeyInput::GetInstance()->Trigger(DIK_TAB))
	{

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


	if (menuExists)
	{
		float posX = 0;
		// countがマックスに到達するまで
		if (easingCount <= MAX_EASING_COUNT)
		{
			posX = Easing::EaseInOutCubic(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
		}
		enemyGuides->SetPosition({ posX,0 });
		easingCount++;
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
