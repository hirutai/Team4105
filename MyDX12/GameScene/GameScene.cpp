#include "GameScene.h"
#include "Title.h"
#include "Select.h"
#include "Play.h"
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
#include "../Tool/Easing.h"
#include "../GameObject/Curtain.h"

#include "../2D/SpriteLoader.h"
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
	for (auto x : curtain) {
		delete x;
	}
	delete gamePad;
	delete audio;
}

void XIIlib::GameScene::ChangeState(SceneState* different_state)
{
	UnitManager::GetInstance()->ResetCount();
	audio->StopBGM();
	delete state;
	state = different_state;
	state->Initialize(this);
}

void XIIlib::GameScene::Initialize()
{
	// Audioの初期化
	audio = new Audio();
	UnitManager::GetInstance()->SetAudio(audio);
	// 前景テクスチャの初期化
	// サイズを設定
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;
	// テクスチャも含めて初期化
	curtain.resize(s_y * s_x);
	for (int i = 0; i < s_y; ++i)
	{
		for (int j = 0; j < s_x; ++j) {
			curtain[j + (i * s_x)] = Curtain::Create(BAN_TEX, (float)(j * tex_size) + (float)(tex_size / 2.0f), (float)(i * tex_size) + (float)(tex_size / 2.0f));
		}
	}
	
	// シーンの初期化
	state->Initialize(this);


	// コメントしました。
}

void XIIlib::GameScene::Update()
{
	//SetCursorPos(0,0);
	//音
	if (KeyInput::GetInstance()->Trigger(DIK_RETURN)) {
		//audio->PlaySE("yankeeVoice.wav");
		//audio->PlaySE("swing.wav");
		
	}

	// GamePadの更新
	gamePad->Update();

	int bai = 16,dist = 32;

	//DebugJISText::GetInstance()->Print("青がルーク", dist, dist * bai, 1); bai++;
	//DebugJISText::GetInstance()->Print("緑がビショップ", dist, dist * bai,1); bai++;
	//DebugJISText::GetInstance()->Print("黄がナイト", dist, dist * bai,1);

	// シーンの更新
	state->CommonUpdate(this);
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
	for (auto i : curtain) {
		i->Draw();
	}
}

void XIIlib::GameScene::DrawBackground()
{
	state->DrawBackground();
}

bool XIIlib::GameScene::DrawScreen(bool on_curtain)
{
	// クローズ
	if (!on_curtain) {
		return ClosedCurtain();
	}
	// オープン
	else {
		return OpenedCurtain();
	}
}

bool XIIlib::GameScene::OpenedCurtain()
{
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;

	struct Index2 {
		int y, x;// 要素数の格納用構造体
	};
	std::vector<Index2> indecis;

	for (int i = 0; i < s_y; ++i)
	{
		for (int j = 0; j < s_x; ++j) {
			if (curtain[j + (i * s_x)]->GetState() == CLOSE) {
				indecis.push_back({ i,j });
			}
		}
	}

	int arraysize = indecis.size();

	if (arraysize != 0) {
		Index2 ind2 = indecis[rand() % arraysize];
		curtain[ind2.x + (ind2.y * s_x)]->SetState(ACTIVE);
	}

	for (auto spr : curtain) {
		spr->Open();
	}

	for (auto spr : curtain) {
		if (spr->GetState() == ACTIVE || spr->GetState() == CLOSE)return false;
	}

	return true;
}

bool XIIlib::GameScene::ClosedCurtain()
{
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;

	struct Index2 {
		int y, x;// 要素数の格納用構造体
	};
	std::vector<Index2> indecis;

	for (int i = 0; i < s_y; ++i) {
		for (int j = 0; j < s_x; ++j) {
			if (curtain[j + (i * s_x)]->GetState() == OPEN) {
				indecis.push_back({ i,j });
			}
		}
	}

	int arraysize = indecis.size();

	if (arraysize != 0) {
		Index2 ind2 = indecis[rand() % arraysize];
		const int i = ind2.x + (ind2.y * s_x);
		curtain[i]->SetState(ACTIVE);
		curtain[i]->SetAlpha(1.0f);
	}

	for (auto spr : curtain) {
		spr->Close();
	}

	for (auto spr : curtain) {
		if (spr->GetState() == OPEN || spr->GetState() == ACTIVE)return false;
	}

	return true;
}

XIIlib::Audio* XIIlib::GameScene::GetAudio() const
{
	return audio;
}

//うぼぁぁぁ