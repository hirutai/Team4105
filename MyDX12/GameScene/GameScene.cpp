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
#include "../GameObject/IntervalTimer.h"
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

XIIlib::GameScene::GameScene()
{ 
	// �R���X�g���N�^!
	state = new Title; // ������Ԃ̊i�[�E�ݒ�
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
	// Audio�̏�����
	audio = new Audio();
	

	// IntervalTimer new�Ə�����
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(2);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);
	// �V�[���̏�����
	state->Initialize(this);

	// �R�����g���܂����B
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
	//	Messenger::GetInstance()->AddPrintOut("�`�������ꂽ�B");
	//}

	int bai = 16,dist = 32;

	DebugJISText::GetInstance()->Print("�����[�N", dist, dist * bai, 1); bai++;
	DebugJISText::GetInstance()->Print("�΂��r�V���b�v", dist, dist * bai,1); bai++;
	DebugJISText::GetInstance()->Print("�����i�C�g", dist, dist * bai,1);

	// �V�[���̍X�V
	state->Update(this);

	intervalTimter->Timer();
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

	intervalTimter->Draw();
}

XIIlib::Audio* XIIlib::GameScene::GetAudio() const
{
	return audio;
}

//���ڂ�����