#include "GameScene.h"
#include "Title.h"
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
	for (auto x : screen_curtain) {
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
	m_curtain.resize(s_y);

	for (int i = 0; i < s_y; i++) {
		m_curtain.at(i).resize(s_x);
	}
	// テクスチャも含めて初期化
	screen_curtain.resize(s_y * s_x);
	time_curtain.resize(s_y * s_x);
	for (int i = 0; i < s_y; ++i)
	{
		for (int j = 0; j < s_x; ++j) {
			m_curtain[i][j] = 0;
			screen_curtain[j + (i * s_x)] =
				Sprite::Create(BAN_TEX, { (float)(j * tex_size) + (float)(tex_size / 2.0f),(float)(i * tex_size) + (float)(tex_size / 2.0f) }, { 1,1,1,1 }, { 0.5f,0.5f });
			screen_curtain[j + (i * s_x)]->SetSize({ 0.0f,0.0f });
			time_curtain[j + (i * s_x)] = 0.0f;
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
	for (auto i : screen_curtain) {
		i->Draw();
	}
}

void XIIlib::GameScene::DrawBackground()
{
	state->DrawBackground();
}

bool XIIlib::GameScene::DrawScreen(bool on_curtain)
{
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;
	// クローズ
	if (!on_curtain) {
		struct Index2 {
			int y, x;// 要素数の格納用構造体
		};
		std::vector<Index2> indecis;

		for (int i = 0; i < s_y; ++i)
		{
			for (int j = 0; j < s_x; ++j) {
				if (m_curtain[i][j] == 0) {
					indecis.push_back({ i,j });
				}
			}
		}

		int arraysize = indecis.size();
		if (arraysize != 0) {
			Index2 ind2 = indecis[rand() % arraysize];
			m_curtain[ind2.y][ind2.x] = 1;
			screen_curtain[ind2.x + (ind2.y * s_x)]->SetColor(1, 1, 1, 1);
		}

		for (int i = 0; i < s_y; ++i) {
			for (int j = 0; j < s_x; ++j) {
				if (m_curtain[i][j] == 1)
				{
					const float ct = 0.25f;
					float newSize = (float)Easing::EaseOutBack((double)time_curtain[j + (i * s_x)], 0, (double)tex_size, (double)ct);
					time_curtain[j + (i * s_x)] += (1.0f / 60.0f);
					screen_curtain[j + (i * s_x)]->SetSize({ newSize ,newSize });
					if (time_curtain[j + (i * s_x)] >= ct) {
						time_curtain[j + (i * s_x)] = 0.0f;
						m_curtain[i][j] = 2;
					}
				}
			}
		}

		for (auto y : m_curtain) {
			for (auto x : y) {
				if (x == 0 || x == 1)return 0;
			}
		}
		return 1;
	}
	// オープン
	else {
		struct Index2 {
			int y, x;// 要素数の格納用構造体
		};
		std::vector<Index2> indecis;

		for (int i = 0; i < s_y; ++i)
		{
			for (int j = 0; j < s_x; ++j) {
				if (m_curtain[i][j] == 2) {
					indecis.push_back({ i,j });
				}
			}
		}

		int arraysize = indecis.size();
		if (arraysize != 0) {
			Index2 ind2 = indecis[rand() % arraysize];
			m_curtain[ind2.y][ind2.x] = 1;
		}

		for (int i = 0; i < s_y; ++i) {
			for (int j = 0; j < s_x; ++j) {
				if (m_curtain[i][j] == 1)
				{
					const float ct = 0.25f;
					float newSize = (float)Easing::EaseInBack((double)time_curtain[j + (i * s_x)], 0, (double)tex_size, ct);
					time_curtain[j + (i * s_x)] += (1.0f / 60.0f);
					float result = (float)tex_size - newSize;
					screen_curtain[j + (i * s_x)]->SetSize({ result ,result });
					if (time_curtain[j + (i * s_x)] >= ct) {
						time_curtain[j + (i * s_x)] = 0.0f;
						screen_curtain[j + (i * s_x)]->SetColor(0, 0, 0, 0);
						m_curtain[i][j] = 0;
					}
				}
			}
		}

		for (auto y : m_curtain) {
			for (auto x : y) {
				if (x == 1 || x == 2)return 0;
			}
		}

		return 1;
	}
}

XIIlib::Audio* XIIlib::GameScene::GetAudio() const
{
	return audio;
}

//うぼぁぁぁ