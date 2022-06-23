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
	// �R���X�g���N�^!
	state = new Title; // ������Ԃ̊i�[�E�ݒ�

	// GamePad�̐����Ə�������State�ɐݒ�
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
	// Audio�̏�����
	audio = new Audio();
	UnitManager::GetInstance()->SetAudio(audio);
	// �O�i�e�N�X�`���̏�����
	// �T�C�Y��ݒ�
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;
	// �e�N�X�`�����܂߂ď�����
	curtain.resize(s_y * s_x);
	for (int i = 0; i < s_y; ++i)
	{
		for (int j = 0; j < s_x; ++j) {
			curtain[j + (i * s_x)] = Curtain::Create(BAN_TEX, (float)(j * tex_size) + (float)(tex_size / 2.0f), (float)(i * tex_size) + (float)(tex_size / 2.0f));
		}
	}
	
	// �V�[���̏�����
	state->Initialize(this);


	// �R�����g���܂����B
}

void XIIlib::GameScene::Update()
{
	//SetCursorPos(0,0);
	//��
	if (KeyInput::GetInstance()->Trigger(DIK_RETURN)) {
		//audio->PlaySE("yankeeVoice.wav");
		//audio->PlaySE("swing.wav");
		
	}

	// GamePad�̍X�V
	gamePad->Update();

	int bai = 16,dist = 32;

	//DebugJISText::GetInstance()->Print("�����[�N", dist, dist * bai, 1); bai++;
	//DebugJISText::GetInstance()->Print("�΂��r�V���b�v", dist, dist * bai,1); bai++;
	//DebugJISText::GetInstance()->Print("�����i�C�g", dist, dist * bai,1);

	// �V�[���̍X�V
	state->CommonUpdate(this);
}

void XIIlib::GameScene::Draw()
{
	// �V�[���̕`��
	state->Draw();
}

void XIIlib::GameScene::DrawSprite()
{
	// �V�[���̃e�N�X�`���`��
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
	// �N���[�Y
	if (!on_curtain) {
		return ClosedCurtain();
	}
	// �I�[�v��
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
		int y, x;// �v�f���̊i�[�p�\����
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
		int y, x;// �v�f���̊i�[�p�\����
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

//���ڂ�����