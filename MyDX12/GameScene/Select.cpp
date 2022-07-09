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

	//画像
	//easyButton.reset(Sprite::Create(EASY_BUTTON_TEX, { HOMEL_POS.x,HOMEL_POS.y +100}));
	easyButton.reset(Sprite::Create(SHITAPPA, { HOMEL_POS.x,HOMEL_POS.y + 100 }));
	//normalButton.reset(Sprite::Create(NORMAL_BUTTON_TEX, { HOMEC_POS.x,HOMEC_POS.y + 100 }));
	normalButton.reset(Sprite::Create(ZOKU, { HOMEC_POS.x,HOMEC_POS.y + 100 }));
	hardButton.reset(Sprite::Create(HARD_BUTTON_TEX, { HOMER_POS.x,HOMER_POS.y + 100 }));
	easyYankeeSilhouette.reset(Sprite::Create(YANKEE_SILHOUETTE, HOMEL_POS));
	easyYankee.reset(Sprite::Create(YANKEE, HOMEL_POS));
	normalYankeeSilhouette.reset(Sprite::Create(BIKE_SILHOUETTE, HOMEC_POS));
	normalYankee.reset(Sprite::Create(BIKE, HOMEC_POS));
	bossSilhouette.reset(Sprite::Create(BOSS_SILHOUETTE, HOMER_POS));
	boss.reset (Sprite::Create(BOSS, HOMER_POS));

	easyButton->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	normalButton->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	hardButton->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	easyYankeeSilhouette->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	easyYankee->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	normalYankeeSilhouette->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	normalYankee->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	bossSilhouette->SetAnchorPoint(Math::Vector2(0.5, 0.5));
	boss->SetAnchorPoint(Math::Vector2(0.5, 0.5));

	easyButton->SetSize({ 300,200 });
	hardButton->SetSize({ 300,200 });
	normalButton->SetSize({ 200,300 });

	edge.reset(Sprite::Create(EDGE_TEX, HOMEL_POS));
	yankee1.reset(Sprite::Create(PLAYER1, HOMEL_POS));
	yankee2.reset(Sprite::Create(PLAYER2, HOMEL_POS));

	bg.reset(Sprite::Create(SELECT_BACK_GROUND, {0, 0}));
	bg_blood.reset(Sprite::Create(BLOOD_BACK_GROUND, { 0,0 }));
	stageSelect_sp.reset(Sprite::Create(STAGESELECT_TEX, { 320, 0}));
	UnitManager::GetInstance()->FlatTileState();
	phase = Phase::CameraDirecting;
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");

	// カメラ設定
	// カメラの初期化
	SetDebugCamera(p_game_scene->GetCamera());
	Math::Vector3 cameraEye = { 0,10,-30 };
	debugCamera->SetPosition(cameraEye.x, cameraEye.y, cameraEye.z); // 視点座標の設定
	debugCamera->_Update();

	SceneState::BackStagesInit();

	stage_easy = false;
	stage_normal = false;
	stage_boss = false;
	select = false;

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
				cursorPos.x -= moveX;
				if (cursorPos.x < HOMEL_POS.x)
				{
					cursorPos.x = HOMEL_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
			else if (KeyInput::GetInstance()->Trigger(DIK_D))
			{
				cursorPos.x += moveX;
				if (cursorPos.x > HOMER_POS.x)
				{
					cursorPos.x = HOMER_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
		}
	}

	// 超えたら戻す
	if (cursorPos.x <= HOMEL_POS.x)
	{
		cursorPos.x = HOMEL_POS.x;
	}
	else if (cursorPos.x >= HOMER_POS.x)
	{
		cursorPos.x = HOMER_POS.x;
	}

	// 情報の更新
	yankee1->SetPosition(cursorPos);
	yankee2->SetPosition(cursorPos);

	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			p_game_scene->ChangeState(new Play);
		}
	}

	if (trigSpace)return;

	if (cursorPos.x == HOMEL_POS.x)
	{
		stage_easy = true;
		stage_normal = false;
		stage_boss = false;
	}
	else if (cursorPos.x == HOMEC_POS.x)
	{
		stage_normal = true;
		stage_easy = false;
		stage_boss = false;
	}
	else if (cursorPos.x == HOMER_POS.x)
	{
		stage_boss = true;
		stage_easy = false;
		stage_normal = false;
	}

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		if (trigSpace)return;
		trigSpace = true;
		select = true;

		// ポジションによってステージナンバーを代入
		if (cursorPos.x == HOMEL_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			SceneState::CreateUnitsPosition(StageNumber::EASY, "stage0");
		}
		else if (cursorPos.x == HOMEC_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			SceneState::CreateUnitsPosition(StageNumber::NORMAL, "stage0");
		}
		else if (cursorPos.x == HOMER_POS.x)
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
	if (stage_easy) {
		easyYankee->Draw();
		if (!select) {
			easyButton->Draw();
		}
	}
	else
	{
		easyYankeeSilhouette->Draw();
	}
	if (stage_normal) {
		normalYankee->Draw();
		if (!select) {
			normalButton->Draw();
		}
	}
	else
	{
		normalYankeeSilhouette->Draw();
	}
	if (stage_boss) {
		boss->Draw();
		if (!select) {
			hardButton->Draw();
		}
	}
	else
	{
		bossSilhouette->Draw();
	}
	if (select) {
		yankee2->Draw();
	}
	else
	{
		yankee1->Draw();
	}
	stageSelect_sp->Draw();
}

void XIIlib::Select::DrawBackground()
{
	bg->Draw();
	if (select) {
		bg_blood->Draw();
	}
}
