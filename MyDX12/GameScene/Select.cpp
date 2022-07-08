#include "Select.h"
#include "GameScene.h"
#include "Play.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../GameObject/UnitManager.h"
#include "../Camera/DebugCamera.h"
#include "../Struct/Math.h"
#include "../3D/Object3D.h"

XIIlib::Select::Select()
{
	// Initializeとほぼ同じ
}

XIIlib::Select::~Select()
{
	// ポインタ使ったやつの埋葬場
}

void XIIlib::Select::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	easyButton.reset(Sprite::Create(EASY_BUTTON_TEX, HOMEL_POS));
	normalButton.reset(Sprite::Create(NORMAL_BUTTON_TEX, HOMEC_POS));
	hardButton.reset(Sprite::Create(HARD_BUTTON_TEX, HOMER_POS));
	edge.reset(Sprite::Create(EDGE_TEX, HOMEL_POS));
	bg_sp.reset(Sprite::Create(BG_TEX, { 0, 0}));
	stageSelect_sp.reset(Sprite::Create(STAGESELECT_TEX, { 250, 0}));
	UnitManager::GetInstance()->FlatTileState();
	phase = Phase::CameraDirecting;
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");

	// カメラ設定
	// カメラの初期化
	SetDebugCamera(p_game_scene->GetCamera());
	Math::Vector3 cameraEye = { 0,10,-30 };
	debugCamera->SetPosition(cameraEye.x, cameraEye.y, cameraEye.z); // 視点座標の設定
	debugCamera->_Update();

}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}

	if (!trigSpace) {
		// AかDを押してたら
		if (KeyInput::GetInstance()->Trigger(DIK_A)
			|| KeyInput::GetInstance()->Trigger(DIK_D))
		{
			const float moveX = BASE_SIZE.x + GAP;

			if (KeyInput::GetInstance()->Trigger(DIK_A))
			{
				edgePos.x -= moveX;
				if (edgePos.x < HOMEL_POS.x)
				{
					edgePos.x = HOMEL_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
			else if (KeyInput::GetInstance()->Trigger(DIK_D))
			{
				edgePos.x += moveX;
				if (edgePos.x > HOMER_POS.x)
				{
					edgePos.x = HOMER_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
		}
	}

	// 超えたら戻す
	if (edgePos.x <= HOMEL_POS.x)
	{
		edgePos.x = HOMEL_POS.x;
	}
	else if (edgePos.x >= HOMER_POS.x)
	{
		edgePos.x = HOMER_POS.x;
	}

	// 情報の更新
	edge->SetPosition(edgePos);

	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			p_game_scene->ChangeState(new Play);
		}
	}

	if (trigSpace)return;

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		if (trigSpace)return;
		trigSpace = true;

		// ポジションによってステージナンバーを代入
		if (edgePos.x == HOMEL_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			SceneState::CreateUnitsPosition(StageNumber::EASY, "stage0");
		}
		else if (edgePos.x == HOMEC_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			SceneState::CreateUnitsPosition(StageNumber::NORMAL, "stage0");
		}
		else if (edgePos.x == HOMER_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav",0.3f);
			SceneState::CreateUnitsPosition(StageNumber::HARD, "stage0");
		}
	}
}

void XIIlib::Select::Draw()
{
	// 3D描画
}

void XIIlib::Select::DrawTex()
{
	// スプライト描画
	easyButton->Draw();
	normalButton->Draw();
	hardButton->Draw();
	stageSelect_sp->Draw();
	edge->Draw();
}

void XIIlib::Select::DrawBackground()
{
	bg_sp->Draw();
}
