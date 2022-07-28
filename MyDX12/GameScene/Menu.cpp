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
#include "../3D/Object3D.h"
#include <cassert>

XIIlib::Menu::Menu()
{
	// Initialize�Ƃقړ���

}

XIIlib::Menu::~Menu()
{
	// �|�C���^�g������̖�����
}

void XIIlib::Menu::Initialize()
{
	// �萔
	const DirectX::XMFLOAT4 color = {1,1,1,1};
	const Math::Vector2 anchorPoint = {0.5f,0.5f};

	// ����
	specterBG.reset(Sprite::Create(WHITEOUT, { 0.0f,0.0f }, {0,0,0,0}));
	specterBG->SetAlpha(0.0f);
	spStageBG1.reset(Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f })); // �w�i
	cursor.reset(Sprite::Create(CURSOR_TEX, { winCenter.x - CURSOR_SPACE, winCenter.y - SPACE }, color, anchorPoint));
	playerGuide.reset(Sprite::Create(PLAYERGUIDES_TEX, { 0.0f,0.0f })); // �v���C���[�̐���
	enemyGuides.reset(Sprite::Create(ENEMYGUIDES_TEX, winCenter, color, anchorPoint)); // �G�̐���
	enemyGuides->SetSize(winCenter);

	rulePos = { winCenter.x, winCenter.y - SPACE * 2.5f };
	if (stageNum == StageNumber::HARD)
	{
		buttons[(int)CursorState::RULE]
			.reset(Sprite::Create(BOSSCLEARCONDITION,rulePos, color, anchorPoint));
	}
	else
	{
		buttons[(int)CursorState::RULE]
			.reset(Sprite::Create(CLEARCONDITION,rulePos, color, anchorPoint));
	}
	buttons[(int)CursorState::CONTINUE] 
		.reset(Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE}, color, anchorPoint));
	buttons[(int)CursorState::PLAYER_GUIDS] 
		.reset(Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y}, color, anchorPoint));
	buttons[(int)CursorState::ENEMY_GUIDS] 
		.reset(Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y + SPACE }, color, anchorPoint));
	buttons[(int)CursorState::NEXT_SLECT] 
		.reset(Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE * 2 }, color, anchorPoint));
	buttons[(int)CursorState::NEXT_TITLE] 
		.reset(Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 3 }, color, anchorPoint));

	//for (int i = 0; i < MAX_BUTTON; i++)
	//{
	//	buttons[i]->SetSize({0.5f,0.5f});
	//}

	// �ő�ŏ��l�����߂�
	CheckLimitPos();

	// �S�ẴT�C�Y��0��
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}
}

void XIIlib::Menu::Update()
{
#pragma region Easing����
	// Easing����
	LerpAlpha();
	endFlag = EasingUpdate();
	// �C�[�W���O�������Ȃ瑦���^�[��
	if (easingState != EasingState::NONE)return;
	if (trigSpace)return;
	if (endFlag)return;
#pragma endregion

#pragma region ����
	// ���������ړ�����
	static Math::Vector3 move;
	if (-600.0f >= rulePos.x)rulePos.x = (winCenter.x * 2) + 300.0f;
	if (count >= moveTime) {
		rulePos.x += VEL_X;
		buttons[0]->SetPosition(rulePos);
	}
	else
	{
		// �k���鏈��
		if (count >= 120) {
			switchRand.x = switchRandiMin + (int)(rand() * (switchRandiMax - switchRandiMin + 1) / (1 + RAND_MAX));
			switchRand.y = switchRandiMin + (int)(rand() * (switchRandiMax - switchRandiMin + 1) / (1 + RAND_MAX));
			buttons[0]->SetPosition({ rulePos.x + switchRand.x,rulePos.y + switchRand.y });
		}
	}
	count++;

	// menu��Ԃ������\���ɂȂ��Ă�����
	if (menuState == MenuState::PLAYER_GUIDS || menuState == MenuState::ENEMY_GUIDS)
	{
		if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || KeyInput::GetInstance()->Trigger(DIK_TAB) 
			|| gamePad_->Button_Down(X_A) || gamePad_->Button_Down(X_START))
		{
			menuState = MenuState::NONE;
			p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
		}
		return;
	}
#pragma endregion

#pragma region ���j���[����
	// �J�[�\���̕\��
	if (!cursorDisp) { cursorDisp = true; }
	
	// �J�[�\���ړ�
	MoveCursor();

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A))
	{
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
		menuState = (MenuState)cursorState;
		if (menuState == MenuState::PLAYER_GUIDS)
		{
			playerGuide->SetPosition({ 0.0f,0.0f });
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
			trigSpace = true;
			easingState = EasingState::MOVE_OUT;
		}
		return;
	}

	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_TAB) || gamePad_->Button_Down(X_START)) {
		easingState = EasingState::MOVE_OUT;
	}
#pragma endregion 
}

void XIIlib::Menu::Draw()
{
	SceneState::BackStagesDraw();
	// 3D�`��
	AttackAreaManager::GetInstance()->Draw();
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Menu::DrawTex()
{
	specterBG->Draw();
	if (menuState == MenuState::PLAYER_GUIDS)
	{
		playerGuide->Draw();
		return;
	}
	else if (menuState == MenuState::ENEMY_GUIDS)
	{
		enemyGuides->Draw();
		return;
	}

	// �X�v���C�g�`��
	if (cursorDisp) {
		cursor->Draw();
	}
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->Draw();
	}
}

void XIIlib::Menu::DrawBackground()
{
	spStageBG1->Draw();
}

bool XIIlib::Menu::EasingUpdate()
{
	// �Ō�̃J�E���^�[��MAX�ɓ��B����܂ŁAEasing����
	if (easingState == EasingState::MOVE_IN || easingState == EasingState::MOVE_OUT)
	{
		// �J�[�\���̔�\��
		if (cursorDisp) { cursorDisp = false; }
		// Easing����
		for (int i = 0; i < MAX_BUTTON; ++i)
		{
			EasingMove(i, easingState);
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
					menuExists = false;
					return true;
					break;
				case XIIlib::MenuState::NEXT_SLECT:
					// �V�[����߂�ۂ�Unit�f�[�^�������Ă���
					UnitManager::GetInstance()->AllDestroy();
					if (trigSpace) {
						if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
							p_game_scene->ChangeState(new Select);
						}
					}
					easyCount = 0;
					menuExists = false;
					return true;
					break;
				case XIIlib::MenuState::NEXT_TITLE:
					// �V�[����߂�ۂ�Unit�f�[�^�������Ă���
					UnitManager::GetInstance()->AllDestroy();
					if (trigSpace) {
						if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
							p_game_scene->ChangeState(new Title);
						}
					}
					easyCount = 0;
					menuExists = false;
					return true;
					break;
				default:
					p_game_scene->ChangeState(new Play);
					break;
				}
				
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
	return false;
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
		if(i == 0)buttons[i]->SetSize(size * 1.5f);
		else buttons[i]->SetSize(size);
		buttons[i]->SetAlpha(alpha);
	}
	else if (easingState == EasingState::MOVE_OUT)
	{
		// ���̕�����
		if (i == 0)buttons[i]->SetSize((defaultSize * 1.5f) - size);
		else buttons[i]->SetSize(defaultSize - size);
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
	float min = buttons[1]->GetPosition().y;
	float max = buttons[1]->GetPosition().y;
	for (int i = 1; i < MAX_BUTTON; ++i)
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
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S)
		|| gamePad_->LStickDownFront() || gamePad_->LStickDownBack())
	{
		// �ړ�����
		float moveY = 0;
		Math::Vector2 pos = cursor->GetPosition();
		// ���ꂼ��̏���
		if (KeyInput::GetInstance()->Trigger(DIK_W) || gamePad_->LStickDownFront())
		{
			moveY = -SPACE;
			p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S) || gamePad_->LStickDownBack())
		{
			moveY = SPACE;
			p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
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

void XIIlib::Menu::LerpAlpha()
{
	static float alpha = 0.0f;
	if (easingState == EasingState::MOVE_IN)
	{
		alpha += 0.01f;
		if (alpha >= 0.4f)alpha = 0.4f;
		specterBG->SetAlpha(alpha);
	}
	else if (easingState == EasingState::MOVE_OUT)
	{
		alpha -= 0.01f;
		if (alpha <= 0.0f)alpha = 0.0f;
		specterBG->SetAlpha(alpha);
	}
	
}

