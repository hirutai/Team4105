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

XIIlib::Play::Play()
{
	// Initializeとほぼ同じ
}

XIIlib::Play::~Play()
{
	// ポインタ使ったやつの埋葬場
	delete backStage;
	delete operatorGuide; // 操作説明
	delete menuButton; // メニュー
	delete spStageBG1;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	SetDebugCamera(p_game_scene->GetCamera());

	if (stageNum == StageNumber::DEBUG)
	{
		// それ以外の数値が入っていたら、仮生成
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
	
	backStage = Object3D::Create(Model::CreateFromOBJ("stage1_all"));
	backStage->scale = Math::Vector3({ 3.0f,3.0f,3.0f });
	backStage->position = Math::Vector3({ 0.0f,-1.0f,0.0f });
	backStage->Update();
	operatorGuide = Sprite::Create(OPERATORGUIDE_TEX, { 1000.0f,600.0f }); // 操作説明
	menuButton = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // メニュー
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");

	count = 0;
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	switch (phase)
	{
	case XIIlib::Phase::CameraDirecting: // カメラ演出
		switch (timing)
		{
		case XIIlib::Timing::ToTheRight: // 右カメラへ
			ToTheRight();
			break;
		case XIIlib::Timing::ToTheBack: // 後ろカメラへ
			ToTheBack();
			break;
		case XIIlib::Timing::ToTheLeft: // 左カメラへ
			ToTheLeft();
			break;
		case XIIlib::Timing::ToTheFront: // 前カメラへ
			ToTheFront();
			break;
		case XIIlib::Timing::ToTheUp: // 上カメラへ
			ToTheUp();
			break;
		}

		debugCamera->SetPosition(cameraEye.x, cameraEye.y, cameraEye.z); // 視点座標の設定
		backStage->Update();
		UnitManager::GetInstance()->ObjectUpdate(); // 3Dオブジェクトの更新
		break;
	case XIIlib::Phase::Game: // ゲーム
#pragma region メニュー処理
		if (!trigSpace) {
			// メニュー画面を展開、閉じる
			if (KeyInput::GetInstance()->Trigger(DIK_TAB))
			{
				p_game_scene->ChangeState(new Menu); // クリアシーンへ
				menuExists = true;
			}
			// メニュー画面を展開するなら即return
			if (menuExists)return;
		}
#pragma endregion 

#pragma region Game Update処理
		backStage->Update();
		count++;
		// 更新
		UnitManager::GetInstance()->Update();
		if (stageNum == StageNumber::HARD && count % 900 == 0)
		{
			std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(0, 7));
			std::shared_ptr<Bishop> bishop1 = std::move(Bishop::Create(7, 7));
			std::shared_ptr<Rook> rook = std::move(Rook::Create(7, 0));
			std::shared_ptr<Rook> rook1 = std::move(Rook::Create(0, 0));
			UnitManager::GetInstance()->AddUnit(std::move(bishop));
			UnitManager::GetInstance()->AddUnit(std::move(rook));
			UnitManager::GetInstance()->AddUnit(std::move(bishop1));
			UnitManager::GetInstance()->AddUnit(std::move(rook1));
		}
		// シーン移動
		if (stageNum == StageNumber::HARD && UnitManager::GetInstance()->GetUnitIDElements("Boss") < 0) {
			trigSpace = true;
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

		if (KeyInput::GetInstance()->Trigger(DIK_C))
		{
			cPushFlag = true;
		}

		if (KeyInput::GetInstance()->Trigger(DIK_O))
		{
			oPushFlag = true;
		}

		if (cPushFlag)
		{
			if (p_game_scene->DrawScreen(TransitionType::WHITE)) {
				p_game_scene->ChangeState(new Clear); // クリアシーンへ

				cPushFlag = false;
				return;
			}
		}
		if (oPushFlag)
		{
			if (p_game_scene->DrawScreen(TransitionType::BLACK)) {
				p_game_scene->ChangeState(new Over); // クリアシーンへ

				oPushFlag = false;
				return;
			}
		}
#pragma endregion
		break;
	}

	debugCamera->_Update(); // 更新
}

void XIIlib::Play::Draw()
{
	// モデルの描画(.obj)
	Object3D::PreDraw();
	backStage->Draw();
	Object3D::PostDraw();
	// 3D描画
	AttackAreaManager::GetInstance()->Draw();
	BillObj::PreDraw();
	HPBar::GetInstance()->Draw();
	BillObj::PostDraw();
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// スプライト描画
	operatorGuide->Draw();
	menuButton->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// 背景スプライト描画
	spStageBG1->Draw();
}

void XIIlib::Play::ToTheRight()
{
	if (cameraEye.x < rightEye.x) // 右側カメラまで
	{
		cameraEye.x += moveValue;
	}
	else
	{
		cameraEye.x = rightEye.x;
	}

	if (cameraEye.z < rightEye.z) // 右側カメラまで
	{
		cameraEye.z += moveValue;
	}
	else
	{
		cameraEye.z = rightEye.z;
	}

	if (cameraEye.x == rightEye.x && cameraEye.z == rightEye.z) // 右側カメラに達したら
	{
		timing = XIIlib::Timing::ToTheBack; // 後ろ側カメラへ
	}
}

void XIIlib::Play::ToTheBack()
{
	if (cameraEye.x > backEye.x) // 後ろ側カメラまで
	{
		cameraEye.x -= moveValue;
	}
	else
	{
		cameraEye.x = backEye.x;
	}

	if (cameraEye.z < backEye.z) // 後ろ側カメラまで
	{

		cameraEye.z += moveValue;
	}
	else
	{
		cameraEye.z = backEye.z;
	}

	if (cameraEye.x == backEye.x && cameraEye.z == backEye.z) // 後ろ側カメラ達したら
	{
		timing = XIIlib::Timing::ToTheLeft; // 左へ
	}
}

void XIIlib::Play::ToTheLeft()
{
	if (cameraEye.x > leftEye.x) // 左側カメラまで
	{
		cameraEye.x -= moveValue; // 移動
	}
	else
	{
		cameraEye.x = leftEye.x;
	}

	if (cameraEye.z > leftEye.z) // 左側カメラまで
	{

		cameraEye.z -= moveValue; // 移動
	}
	else
	{
		cameraEye.z = leftEye.z;
	}

	if (cameraEye.x == leftEye.x && cameraEye.z == leftEye.z) // 左側カメラに達したら
	{
		timing = XIIlib::Timing::ToTheFront; // 前へ
	}
}

void XIIlib::Play::ToTheFront()
{
	if (cameraEye.x < frontEye.x) // 前側カメラまで
	{
		cameraEye.x += moveValue; // 移動
	}
	else
	{
		cameraEye.x = frontEye.x;
	}

	if (cameraEye.z > frontEye.z) // 前側カメラまで
	{

		cameraEye.z -= moveValue; // 移動
	}
	else
	{
		cameraEye.z = frontEye.z;
	}

	if (cameraEye.x == frontEye.x && cameraEye.z == frontEye.z) // 前側カメラに達したら
	{
		timing = XIIlib::Timing::ToTheUp; // 上カメラへ
	}
}

void XIIlib::Play::ToTheUp()
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
		phase = XIIlib::Phase::Game; // ゲームへ
	}
}
