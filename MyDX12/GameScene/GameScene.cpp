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
	// �R���X�g���N�^!
	state = new Title; // ������Ԃ̊i�[�E�ݒ�

	// GamePad�̐����Ə�������State�ɐݒ�
	gamePad = new GamePAD_XInput();
	gamePad->Initialize();
	state->SetGamePad(gamePad);
}

XIIlib::GameScene::~GameScene()
{
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
	
	// �V�[���̏�����
	state->Initialize(this);

	UnitManager::GetInstance()->SetAudio(audio);
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
	state->Update(this);
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
}

void XIIlib::GameScene::DrawBackground()
{
	state->DrawBackground();
}

XIIlib::Audio* XIIlib::GameScene::GetAudio() const
{
	return audio;
}

//���ڂ�����