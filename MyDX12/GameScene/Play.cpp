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

XIIlib::Play::Play()
{
	// Initializeとほぼ同じ

}

XIIlib::Play::~Play()
{
	// ポインタ使ったやつの埋葬場
	delete intervalTimter;
	delete spStageBG1;
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
		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
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

		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
	}
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	// 更新
	UnitManager::GetInstance()->Update();
	AttackAreaManager::GetInstance()->Draw();

	intervalTimter->Timer();

	if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // プレイヤが存在している場合
	{
		if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // 敵を全滅させた時
		{
			p_game_scene->ChangeState(new Clear); // クリアシーンへ
		}
	}

	if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // プレイヤが存在していない場合
	{
		p_game_scene->ChangeState(new Over); // オーバーシーンへ
	}
}

void XIIlib::Play::Draw()
{
	// 3D描画
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// スプライト描画
	//intervalTimter->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// 背景スプライト描画
	spStageBG1->Draw();
}
