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
	// Initialize�Ƃقړ���

}

XIIlib::Play::~Play()
{
	// �|�C���^�g������̖�����
	delete operatorGuide; // �������
	delete menuButton; // ���j���[
	delete spStageBG1;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	if (stageNum == StageNumber::DEBUG)
	{
		// ����ȊO�̐��l�������Ă�����A������
		SceneState::CreateUnitsPosition(StageNumber::DEBUG);
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
		stageNum = StageNumber::NONE;
	}
	else if (stageNum == StageNumber::EASY)
	{
		// �w�i�摜����
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::NORMAL)
	{
		// �w�i�摜����
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::HARD)
	{
		// �w�i�摜����
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	else
	{
		spStageBG1 = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f });
	}
	

	operatorGuide = Sprite::Create(OPERATORGUIDE_TEX, { 1000.0f,600.0f }); // �������
	menuButton = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // ���j���[
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	switch (timing)
	{
	case XIIlib::Timing::CameraDirecting: // �J�������o
		if (cameraEye.x > finalEye)
		{
			cameraEye.x -= cameraRotation; // �c�����ɂȂ�܂ŉ�]
		}
		else
		{
			cameraEye.x = finalEye; // �c�����ɌŒ�
			timing = XIIlib::Timing::Game; // �L�b�N�I�t���o��
		}

		//Camera::GetInstance()->SetEye(XMFLOAT3(cameraEye.x, 30.0f, VERTICALEYE - cameraEye.x));

		//Camera::GetInstance()->SetTarget(XMFLOAT3(cameraEye.x / 4, 0.0f, 0.0f));
		break;
	case XIIlib::Timing::Game: // �Q�[��
#pragma region ���j���[����
		// ���j���[��ʂ�W�J�A����
		if (KeyInput::GetInstance()->Trigger(DIK_TAB))
		{
			p_game_scene->ChangeState(new Menu); // �N���A�V�[����
			menuExists = true;
		}
		// ���j���[��ʂ�W�J����Ȃ瑦return
		if (menuExists)return;
#pragma endregion 

#pragma region Game Update����
		// �X�V
		UnitManager::GetInstance()->Update();
		// �V�[���ړ�
		if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // �v���C�������݂��Ă���ꍇ
		{
			if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // �G��S�ł�������
			{
				trigSpace = true;
			}
		}
		else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // �v���C�������݂��Ă��Ȃ��ꍇ
		{
			trigSpace = true;
		}

		if (trigSpace) {
			if (p_game_scene->DrawScreen(false)) {
				if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // �v���C�������݂��Ă���ꍇ
				{
					if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // �G��S�ł�������
					{
						p_game_scene->ChangeState(new Clear); // �N���A�V�[����
					}
				}
				else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // �v���C�������݂��Ă��Ȃ��ꍇ
				{
					p_game_scene->ChangeState(new Over); // �I�[�o�[�V�[����
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
	// 3D�`��
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// �X�v���C�g�`��
	operatorGuide->Draw();
	menuButton->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// �w�i�X�v���C�g�`��
	spStageBG1->Draw();
}
