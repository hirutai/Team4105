#include "GameScene.h"
#include "Title.h"
#include "../2D/Sprite.h"
#include "../Tool/DigitalNumberText.h"
#include "../Audio/Audio.h"
#include "../Struct/Math.h"
#include "../3D/CollBox.h"
#include "../3D/CollCapsule.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../Input/KeyInput.h"
#include "../Input/ControllerInput.h"
#include "../Tool/Messenger.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

XIIlib::GameScene::GameScene()
{ 
	// コンストラクタ!
	state = new Title; // 初期状態の格納・設定

	// GamePadの生成と初期化とStateに設定
	gamePad = new GamePAD_XInput();
	gamePad->Initialize();
	state->SetGamePad(gamePad);
}

XIIlib::GameScene::~GameScene()
{
	delete audio;
}

void XIIlib::GameScene::ChangeState(SceneState* different_state)
{
	delete state;
	state = different_state;
	state->Initialize(this);
}

void XIIlib::GameScene::Initialize()
{
	// Audioの初期化
	audio = new Audio();
	
	// シーンの初期化
	state->Initialize(this);

	// コメントしました。
}

void XIIlib::GameScene::Update()
{
	//XINPUT_STATE state;
	//ZeroMemory(&state, sizeof(XINPUT_STATE));

	//// Simply get the state of the controller from XInput.
	//DWORD dwResult = XInputGetState(0, &state);

	//if (dwResult == ERROR_SUCCESS)
	//{
	//	// Controller is connected
	//}
	//else
	//{
	//	// Controller is not connected
	//}

	//if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
	//	Messenger::GetInstance()->AddPrintOut("Ａが押された。");
	//}

	// GamePadの更新
	gamePad->Update();

	int bai = 16,dist = 32;

	DebugJISText::GetInstance()->Print("青がルーク", dist, dist * bai, 1); bai++;
	DebugJISText::GetInstance()->Print("緑がビショップ", dist, dist * bai,1); bai++;
	DebugJISText::GetInstance()->Print("黄がナイト", dist, dist * bai,1);

	// シーンの更新
	state->Update(this);
}

void XIIlib::GameScene::Draw()
{
	// シーンの描画
	state->Draw();
}

void XIIlib::GameScene::DrawSprite()
{
	// シーンのテクスチャ描画
	state->DrawTex();
}

XIIlib::Audio* XIIlib::GameScene::GetAudio() const
{
	return audio;
}

//うぼぁぁぁ