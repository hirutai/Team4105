#include "Play.h"
#include "GameScene.h"
#include "Menu.h"
#include "Clear.h"
#include "Over.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
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
	delete operatorGuide; // 操作説明
	delete menuButton; // メニュー
	delete spStageBG1;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	if (stageNum == StageNumber::DEBUG)
	{
		// それ以外の数値が入っていたら、仮生成
		SceneState::CreateUnitsPosition(StageNumber::DEBUG);
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
		stageNum = StageNumber::NONE;
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
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	switch (timing)
	{
	case XIIlib::Timing::CameraDirecting: // カメラ演出
		if (cameraEye.x > finalEye)
		{
			cameraEye.x -= cameraRotation; // 縦向きになるまで回転
		}
		else
		{
			cameraEye.x = finalEye; // 縦向きに固定
			timing = XIIlib::Timing::Game; // キックオフ演出へ
		}

		//Camera::GetInstance()->SetEye(XMFLOAT3(cameraEye.x, 30.0f, VERTICALEYE - cameraEye.x));

		//Camera::GetInstance()->SetTarget(XMFLOAT3(cameraEye.x / 4, 0.0f, 0.0f));
		break;
	case XIIlib::Timing::Game: // ゲーム
#pragma region メニュー処理
		// メニュー画面を展開、閉じる
		if (KeyInput::GetInstance()->Trigger(DIK_TAB))
		{
			p_game_scene->ChangeState(new Menu); // クリアシーンへ
			menuExists = true;
		}
		// メニュー画面を展開するなら即return
		if (menuExists)return;
#pragma endregion 

#pragma region Game Update処理
		// 更新
		UnitManager::GetInstance()->Update();
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
#pragma endregion
		break;
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
	operatorGuide->Draw();
	menuButton->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// 背景スプライト描画
	spStageBG1->Draw();
}
