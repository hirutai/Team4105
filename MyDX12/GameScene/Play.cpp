#include "Play.h"
#include "GameScene.h"
#include "Menu.h"
#include "Clear.h"
#include "Over.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../GameObject/HPBar.h"
#include"../GameObject/AttackAreaManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../Tool/Easing.h"
#include "../Camera/DebugCamera.h"
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include"../GameObject/Bishop.h"
#include"../GameObject/Rook.h"
#include"../GameObject/Yankee.h"

XIIlib::Play::Play()
{
	// Initializeとほぼ同じ
}

XIIlib::Play::~Play()
{
	// ポインタ使ったやつの埋葬場
	delete operatorGuide; // 操作説明
	delete menuButton; // メニュー
	delete spStageBG1;

	delete clearCond;
	delete bossClearCond;
}

void XIIlib::Play::Initialize()
{
	SetDebugCamera(p_game_scene->GetCamera());

	if (stageNum == StageNumber::DEBUG)
	{
		// それ以外の数値が入っていたら、仮生成
		// Object3Dの初期化
		SceneState::BackStagesInit();

		SceneState::CreateUnitsPosition(StageNumber::DEBUG,"stage0");
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
		stageNum = StageNumber::NONE;
		phase = Phase::Game;
		debugCamera->SetPosition(upperEye.x, upperEye.y, upperEye.z); // 視点座標の設定
	}
	else if (stageNum == StageNumber::EASY)
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
	else
	{
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	
	operatorGuide = Sprite::Create(OPERATORGUIDE_TEX, { 1000.0f,600.0f }); // 操作説明
	menuButton = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // メニュー

	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");

	count = 0;

	clearCond = Sprite::Create(CLEARCONDITION, { 1280.0f, 768.0f / 2 }); // クリア条件画像の作成
	clearCond->SetAnchorPoint({ 0.5f, 0.5f }); // 中心に設定

	bossClearCond = Sprite::Create(BOSSCLEARCONDITION, { 1280.0f, 768.0f / 2 }); // ボスクリア条件画像の作成
	bossClearCond->SetAnchorPoint({ 0.5f, 0.5f }); // 中心に設定
}

void XIIlib::Play::Update()
{
	switch (phase)
	{
	case XIIlib::Phase::CameraDirecting: // カメラ演出
		RotateEye();

		break;
	case XIIlib::Phase::ClearCondDisplay: // クリア条件表示
		UnitManager::GetInstance()->ObjectUpdate(); // 3Dオブジェクトの更新

		if (stageNum == StageNumber::HARD) { // HARD時
			ClearCondDisplay(bossClearCond); // ボスクリア条件の表示
		}
		else { // HARD以外
			ClearCondDisplay(clearCond);// クリア条件の表示
		}

		break;
	case XIIlib::Phase::Game: // ゲーム
#pragma region メニュー処理
		if (!trigSpace) {
			// メニュー画面を展開、閉じる
			if (KeyInput::GetInstance()->Trigger(DIK_TAB) || gamePad_->Button_Down(X_START))
			{
				p_game_scene->ChangeState(new Menu); // クリアシーンへ
				menuExists = true;
			}
			// メニュー画面を展開するなら即return
			if (menuExists)return;
		}
#pragma endregion 

#pragma region Game Update処理
		
		count++;
		// 更新
		UnitManager::GetInstance()->Update();
		//
		int createMin = Min;
		int createMax = Max;
		tileRand = 1;

		std::cout << "easyCount" << easyCount<<std::endl;
		if (stageNum == StageNumber::EASY && UnitManager::GetInstance()->GetUnitIDElements("King") >= 0)
		{
			if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // 敵を全滅させた時
			{
				appearCount++;
				if (easyCount == 0 && appearCount == 60)
				{
					std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(4, 7));
					std::shared_ptr<Yankee> yankee1 = std::move(Yankee::Create(4, 5));			
					UnitManager::GetInstance()->AddUnit(std::move(yankee));
					UnitManager::GetInstance()->AddUnit(std::move(yankee1));
					UnitManager::GetInstance()->Update();
					appearCount = 0;
					easyCount += 1;
				}
				else if (easyCount == 1 && appearCount == 60)
				{
					std::shared_ptr<Rook> rook = std::move(Rook::Create(3, 6));
					UnitManager::GetInstance()->AddUnit(std::move(rook));
					UnitManager::GetInstance()->Update();
					appearCount = 0;
					easyCount += 1;
				}
			}
		}

		if (stageNum == StageNumber::HARD && count % 900 == 0)
		{
			tileRand = Min + (int)(rand() * (Max - Min + 1) / (1 + RAND_MAX));
			if (tileRand == 0)
			{

				std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(0, 7));
				std::shared_ptr<Bishop> bishop1 = std::move(Bishop::Create(7, 7));
				std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 0));
				std::shared_ptr<Rook> rook1 = std::move(Rook::Create(3, 0));
				UnitManager::GetInstance()->AddUnit(std::move(bishop));
				UnitManager::GetInstance()->AddUnit(std::move(rook));
				UnitManager::GetInstance()->AddUnit(std::move(bishop1));
				UnitManager::GetInstance()->AddUnit(std::move(rook1));

			}
			if (tileRand == 1)
			{				
				std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(0, 0));
				std::shared_ptr<Bishop> bishop1 = std::move(Bishop::Create(7, 0));
				std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 0));
				std::shared_ptr<Rook> rook1 = std::move(Rook::Create(3, 0));
				UnitManager::GetInstance()->AddUnit(std::move(bishop));
				UnitManager::GetInstance()->AddUnit(std::move(rook));
				UnitManager::GetInstance()->AddUnit(std::move(bishop1));
				UnitManager::GetInstance()->AddUnit(std::move(rook1));
			}
			if (tileRand == 2)
			{
				std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(0, 6));
				std::shared_ptr<Yankee> yankee1 = std::move(Yankee::Create(0, 5));
				std::shared_ptr<Yankee> yankee2 = std::move(Yankee::Create(0, 4));
				std::shared_ptr<Yankee> yankee3 = std::move(Yankee::Create(7, 6));
				std::shared_ptr<Yankee> yankee4 = std::move(Yankee::Create(7, 5));
				std::shared_ptr<Yankee> yankee5 = std::move(Yankee::Create(7, 4));
				UnitManager::GetInstance()->AddUnit(std::move(yankee));
				UnitManager::GetInstance()->AddUnit(std::move(yankee1));
				UnitManager::GetInstance()->AddUnit(std::move(yankee2));
				UnitManager::GetInstance()->AddUnit(std::move(yankee3));
				UnitManager::GetInstance()->AddUnit(std::move(yankee4));
				UnitManager::GetInstance()->AddUnit(std::move(yankee5));
			}
			if (tileRand == 3)
			{
				std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(0, 3));
				std::shared_ptr<Bishop> bishop1 = std::move(Bishop::Create(7, 4));
				std::shared_ptr<Rook> rook = std::move(Rook::Create(0, 4));
				std::shared_ptr<Rook> rook1 = std::move(Rook::Create(7, 3));
				UnitManager::GetInstance()->AddUnit(std::move(bishop));
				UnitManager::GetInstance()->AddUnit(std::move(rook));
				UnitManager::GetInstance()->AddUnit(std::move(bishop1));
				UnitManager::GetInstance()->AddUnit(std::move(rook1));
			}
		}
		// シーン移動
		if (stageNum == StageNumber::HARD && UnitManager::GetInstance()->GetUnitIDElements("Boss") < 0) {
			trigSpace = true;
		}
		else if (stageNum == StageNumber::EASY && UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) {
			if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0 && easyCount >= 2) // 敵を全滅させた時
			{
				trigSpace = true;
			}
		}
		else  if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // プレイヤが存在している場合
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

		if (trigSpace) {
				if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // プレイヤが存在している場合
				{
					//if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // 敵を全滅させた時
					//{
					//	if (p_game_scene->DrawScreen(TransitionType::WHITE)) {
					//		p_game_scene->ResetAlpha();
					//		p_game_scene->ChangeState(new Clear); // クリアシーンへ
					//		return;
					//	}
					//}
					if (p_game_scene->DrawScreen(TransitionType::WHITE)) {
						p_game_scene->ResetAlpha();
						p_game_scene->ChangeState(new Clear); // クリアシーンへ
						return;
					}
				}
				else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // プレイヤが存在していない場合
				{
					if (p_game_scene->DrawScreen(TransitionType::BLACK)) {
						p_game_scene->ResetAlpha();
						p_game_scene->ChangeState(new Over); // オーバーシーンへ
						return;
					}
				}
		}
#pragma endregion
		break;
	}

	debugCamera->_Update(); // 更新
}

void XIIlib::Play::Draw()
{
	SceneState::BackStagesDraw();
	// 3D描画
	AttackAreaManager::GetInstance()->Draw();
	
	UnitManager::GetInstance()->Draw();

	// カメラが移動する際にビルボードオブジェクトの表示をしない
	if (phase == XIIlib::Phase::CameraDirecting)return;
	UnitManager::GetInstance()->BillDraw();

	// ボス戦で無ければ描画処理はしない
	if (stageNum != StageNumber::HARD)return;
	BillObj::PreDraw();
	HPBar::GetInstance()->Draw();
	BillObj::PostDraw();
}

void XIIlib::Play::DrawTex()
{
	// スプライト描画
	operatorGuide->Draw();
	menuButton->Draw();
	if (phase == XIIlib::Phase::ClearCondDisplay) {
		if (stageNum == StageNumber::HARD) { // HARD時
			bossClearCond->Draw();
		}
		else { //HARD以外
			clearCond->Draw();
		}
	}
}

void XIIlib::Play::DrawBackground()
{
	// 背景スプライト描画
	spStageBG1->Draw();
}

void XIIlib::Play::RotateEye()
{
	UnitManager::GetInstance()->ObjectUpdate(); // 3Dオブジェクトの更新

	s = sin(358.0f * 3.1415f / 180);
	c = cos(358.0f * 3.1415f / 180);

	cameraEye.x = s * cameraEye.z + c * cameraEye.x;
	cameraEye.z = c * cameraEye.z - s * cameraEye.x;

	if (rotateCount == upTiming) {
		UpEye();
	}
	else {
		rotateCount = rotateCount + 1;
	}

	debugCamera->SetPosition(cameraEye.x, cameraEye.y, cameraEye.z); // 視点座標の設定
}

void XIIlib::Play::UpEye()
{
	if (cameraEye.y < upperEye.y) // 上カメラまで
	{
		cameraEye.y += moveValue;
	}
	else
	{
		cameraEye.y = upperEye.y;
	}

	if (cameraEye.y == upperEye.y) // 上側カメラに達したら
	{
		phase = XIIlib::Phase::ClearCondDisplay; // クリア条件表示へ
	}
}

void XIIlib::Play::ClearCondDisplay(Sprite* clearCond)
{
	if (clearCondPos.x > 1280.0f / 2) // クリア条件画像が画面中央に止まるまで
	{
		clearCondPos.x -= clearCondVel;
	}
	else
	{
		stopTime++; // 停止時間をカウント

		if (stopTime > 120) // ２秒経過したら
		{
			if (clearCondPos.x > 0) // キックオフの文字が左端に到達するまで
			{
				clearCondPos.x -= clearCondVel;
			}
			else
			{
				phase = XIIlib::Phase::Game; // ゲームへ
			}
		}
	}

	clearCond->SetPosition({ clearCondPos.x, clearCondPos.y });
}
