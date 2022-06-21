#include "Play.h"
#include "GameScene.h"
#include "Menu.h"
#include "Clear.h"
#include "Over.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../GameObject/Yankee.h"
#include "../GameObject/Stone.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../GameObject/IntervalTimer.h"
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
	delete enemyGuides; // �G�̐���
	delete operatorGuide; // �������
	delete menu; // ���j���[
	delete spStageBG1;
	delete intervalTimter;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	
	// IntervalTimer new�Ə�����
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(4, 5);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);
	
	if (stageNum == StageNumber::EASY)
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

	operatorGuide = Sprite::Create(PLAYERGUIDE_TEX, { 1000.0f,600.0f }); // �������
	menu = Sprite::Create(MENU_TEX, { 0.0f,10.0f }); // ���j���[
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, {0.0f,0.0f});; // �G�̐���
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
#pragma region ���j���[����

	// ���j���[��ʂ�W�J�A����
	if (KeyInput::GetInstance()->Trigger(DIK_TAB))
	{
		p_game_scene->ChangeState(new Menu); // �N���A�V�[����
		if (menuExists && easingCount >= MAX_EASING_COUNT)
		{
			// �[��Clear
			easingCount = 0;
			menuExists = false;
		}
		else
		{
			menuExists = true;
		}
	}

	if (menuExists)
	{
		float posX = 0;
		float posY = 0;
		// count���}�b�N�X�ɓ��B����܂�
		if (easingCount <= MAX_EASING_COUNT)
		{
			posX = Easing::EaseInOutElastic(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
			posY = Easing::EaseInOutElastic(easingCount, -winSize.y, winSize.y, MAX_EASING_COUNT);
			easingCount++;
		}
		enemyGuides->SetPosition({ posX,posY });
	}

	// ���j���[���W�J����Ă���Ȃ�return
	if (menuExists)return;
#pragma endregion 

#pragma region Game Update����
	// �X�V
	UnitManager::GetInstance()->Update();
	intervalTimter->Timer();
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
#pragma endregion
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
	//intervalTimter->Draw();
	operatorGuide->Draw();
	menu->Draw();
	// ���j���[���W�J����Ă��Ȃ��Ȃ�return
	if (!menuExists)return;
	enemyGuides->Draw();
	menu->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// �w�i�X�v���C�g�`��
	spStageBG1->Draw();
}
