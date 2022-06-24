#include "Menu.h"
#include "GameScene.h"
#include "Select.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"
#include "Play.h"
#include "Select.h"
#include "Title.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::Menu::Menu()
{
	// Initialize�Ƃقړ���

}

XIIlib::Menu::~Menu()
{
	// �|�C���^�g������̖�����
	delete enemyGuides;
	delete playerGuide;
	for (int i = MAX_BUTTON -1; i >= 0; --i)
	{
		delete buttons[i];
	}
	delete cursor;
	delete spStageBG1;
}

void XIIlib::Menu::Initialize(GameScene* p_game_scene)
{
	// �萔
	const DirectX::XMFLOAT4 color = {1,1,1,1};
	const Math::Vector2 anchorPoint = {0.5f,0.5f};

	// ����
	spStageBG1   = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f }); // �w�i
	cursor       = Sprite::Create(CURSOR_TEX, { winCenter.x - 80.0f, winCenter.y - SPACE * 2 }, color, anchorPoint);
	playerGuide = Sprite::Create(PLAYERGUIDES_TEX, winCenter, color, anchorPoint); // �v���C���[�̐���
	playerGuide->SetSize(winCenter);
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, winCenter, color, anchorPoint); // �G�̐���
	enemyGuides->SetSize(winCenter);

	buttons[(int)CursorState::CONTINUE] 
		= Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE * 2}, color, anchorPoint);
	buttons[(int)CursorState::PLAYER_GUIDS] 
		= Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y - SPACE}, color, anchorPoint);
	buttons[(int)CursorState::ENEMY_GUIDS] 
		= Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y }, color, anchorPoint);
	buttons[(int)CursorState::NEXT_SLECT] 
		= Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE}, color, anchorPoint);
	buttons[(int)CursorState::NEXT_TITLE] 
		= Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 2 }, color, anchorPoint);

	// �ő�ŏ��l�����߂�
	CheckLimitPos();

	// �S�ẴT�C�Y��0��
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}
}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
#pragma region Easing����
	// �Ō�̃J�E���^�[��MAX�ɓ��B����܂ŁAEasing����
	if (easingState == EasingState::MOVE_IN || easingState == EasingState::MOVE_OUT)
	{
		// �J�[�\���̔�\��
		if (cursorDisp) { cursorDisp = false; }
		// Easing����
		for (int i = 0; i < MAX_BUTTON; ++i)
		{
			EasingMove(i,easingState);
		}
		// �J�E���g�̍X�V
		CountsUpdate();

		// �Ō�̃J�E���g���}�b�N�X�ɂȂ�����Easing�̏�Ԃ�NONE
		if (easingCounts[MAX_BUTTON - 1] > MAX_EASING_COUNT)
		{
			// EasingOut�Ȃ�ʃV�[���Ɉړ�
			if (easingState == EasingState::MOVE_OUT)
			{
				switch (menuState)
				{
				case XIIlib::MenuState::CONTINUE:
					p_game_scene->ChangeState(new Play);
					break;
				case XIIlib::MenuState::NEXT_SLECT:
					// �V�[����߂�ۂ�Unit�f�[�^�������Ă���
					UnitManager::GetInstance()->AllDestroy();
					p_game_scene->ChangeState(new Select);
					break;
				case XIIlib::MenuState::NEXT_TITLE:
					// �V�[����߂�ۂ�Unit�f�[�^�������Ă���
					UnitManager::GetInstance()->AllDestroy();
					p_game_scene->ChangeState(new Title);
					break;
				default:
					p_game_scene->ChangeState(new Play);
					break;
				}
				
				menuExists = false;
				return;
			}
			// �J�E���g�����ׂ�0
			for (int i = 0; i < MAX_BUTTON; ++i)
			{
				easingCounts[i] = 0;
			}
			// ��Ԃ�NONE��
			easingState = EasingState::NONE;
		}
	}
	
	if (easingState != EasingState::NONE)return;
#pragma endregion

#pragma region ����
	// menu��Ԃ������\���ɂȂ��Ă�����
	if (menuState == MenuState::PLAYER_GUIDS || menuState == MenuState::ENEMY_GUIDS)
	{
		if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || KeyInput::GetInstance()->Trigger(DIK_TAB))
		{
			menuState = MenuState::NONE;
		}
		return;
	}
#pragma endregion

#pragma region ���j���[����
	// �J�[�\���̕\��
	if (!cursorDisp) { cursorDisp = true; }
	
	// �J�[�\���ړ�
	MoveCursor();

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE))
	{
		menuState = (MenuState)cursorState;
		if (menuState == MenuState::PLAYER_GUIDS)
		{
			playerGuide->SetPosition(winCenter);
		}
		else if (menuState == MenuState::ENEMY_GUIDS)
		{
			enemyGuides->SetPosition(winCenter);
		}
		// �V�[���ړ��֘A�Ȃ�
		else if (cursorState == CursorState::CONTINUE ||
			cursorState == CursorState::NEXT_SLECT ||
			cursorState == CursorState::NEXT_TITLE)
		{
			easingState = EasingState::MOVE_OUT;
		}
		return;
	}

	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_TAB)) {
		easingState = EasingState::MOVE_OUT;
	}
	// �e�X�g�p
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}
#pragma endregion 
}

void XIIlib::Menu::Draw()
{
	// 3D�`��
	AttackAreaManager::GetInstance()->Draw();
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Menu::DrawTex()
{
	// �X�v���C�g�`��
	if (cursorDisp) {
		cursor->Draw();
	}
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->Draw();
	}

	if (menuState == MenuState::PLAYER_GUIDS)
	{
		playerGuide->Draw();
	}
	else if (menuState == MenuState::ENEMY_GUIDS)
	{
		enemyGuides->Draw();
	}

}

void XIIlib::Menu::DrawBackground()
{
	spStageBG1->Draw();
}

void XIIlib::Menu::EasingMove(int i,EasingState easingState)
{
	// count���}�b�N�X�ɓ��B����܂�
	if (easingCounts[i] > MAX_EASING_COUNT) return;
	const Math::Vector2 defaultSize = buttons[i]->GetDefault();
	Math::Vector2 size = { 0,0 };
	float alpha = 0;
	size.x = Easing::InOutCubic(easingCounts[i], 0, defaultSize.x, MAX_EASING_COUNT);
	size.y = Easing::InOutCubic(easingCounts[i], 0, defaultSize.y, MAX_EASING_COUNT);
	alpha = Easing::InOutCubic(easingCounts[i], 0, 1, MAX_EASING_COUNT);
	if (easingState == EasingState::MOVE_IN)
	{
		// 0 + ...�Ȃ̂ł��̂܂�
		buttons[i]->SetSize(size);
		buttons[i]->SetAlpha(alpha);
	}
	else if (easingState == EasingState::MOVE_OUT)
	{
		// ���̕�����
		buttons[i]->SetSize(defaultSize - size);
		buttons[i]->SetAlpha(1 - alpha);
	}
}

void XIIlib::Menu::CountsUpdate()
{
	// �S�ẴJ�E���g�C���N�������g
	int count = 0;
	CountUpdate(count);
}

void XIIlib::Menu::CountUpdate(int& count)
{
	// �J�E���g���C���N�������g�A�x�����̃J�E���g���傫���Ȃ����玟�ɍs��
	easingCounts[count]++;
	if (easingCounts[count] < EASING_SPACE)return;
	count++;
	if (count >= MAX_BUTTON)return;
	CountUpdate(count);
}

void XIIlib::Menu::CheckLimitPos()
{
	// �ő�ƍŏ��𒲂ׂ�
	float min = buttons[0]->GetPosition().y;
	float max = buttons[0]->GetPosition().y;
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		if (min > buttons[i]->GetPosition().y)
		{
			min = buttons[i]->GetPosition().y;
		}

		if (max < buttons[i]->GetPosition().y)
		{
			max = buttons[i]->GetPosition().y;
		}
	}
	// ���
	minPosY = min;
	maxPosY = max;
}

void XIIlib::Menu::MoveCursor()
{
	prevPos = cursor->GetPosition();
	// ����key����������
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S))
	{
		// �ړ�����
		float moveY = 0;
		Math::Vector2 pos = cursor->GetPosition();
		// ���ꂼ��̏���
		if (KeyInput::GetInstance()->Trigger(DIK_W))
		{
			moveY = -SPACE;
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S))
		{
			moveY = SPACE;
		}
		pos.y += moveY;
		// �g�𒴂�����~�߂�
		if (pos.y < minPosY)
		{
			pos.y = minPosY;
		}
		else if (pos.y > maxPosY)
		{
			pos.y = maxPosY;
		}
		// ���W��ݒ�
		cursor->SetPosition(pos);
	}
	// ���j���[�̏��
	CheckMenuState();
}

void XIIlib::Menu::CheckMenuState()
{
	// �ߎ��l
	float nearValue = 5;
	float posY = cursor->GetPosition().y;
	// �O��Ɠ����Ȃ瑦���^�[��
	if (posY == prevPos.y)return;
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		if (posY <= buttons[i]->GetPosition().y + nearValue && posY >= buttons[i]->GetPosition().y - nearValue)
		{
			cursorState = (CursorState)i;
			return;
		}
	}
}

