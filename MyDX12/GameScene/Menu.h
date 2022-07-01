#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
	// �J�[�\�����
	enum class CursorState : int
	{
		NONE = -1,
		CONTINUE,
		PLAYER_GUIDS,
		ENEMY_GUIDS,
		NEXT_SLECT,
		NEXT_TITLE,
	};

	// ���j���[���
	enum class MenuState : int
	{
		NONE = -1,
		CONTINUE,
		PLAYER_GUIDS,
		ENEMY_GUIDS,
		NEXT_SLECT,
		NEXT_TITLE,
	};

	enum class EasingState : int
	{
		NONE = 0,
		MOVE_IN,
		MOVE_OUT,
	};
	// ���j���[�V�[���̃N���X
	class Menu : public SceneState {
	private: // �����o�萔
		const float MAX_EASING_COUNT = 20.0f; // Easing�̃}�b�N�X�J�E���g(20)
		const float CURSOR_SPACE = 250.0f; // �J�[�\���ƕ����̊Ԋu
		const float SPACE = 120.0f; // �Ԋu
		const int  EASING_SPACE = 3; // �Ԋu
		const Math::Vector2 winSize = { 1280.0f,768.0f }; // ��ʃT�C�Y
		const Math::Vector2 winCenter = { winSize.x/2,winSize.y / 2 }; // ��ʂ̒���
	public:
		static const int MAX_BUTTON = 5; // �X�v���C�gButton�̌�

	private:
		// ���j���[���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		CursorState cursorState = CursorState::CONTINUE; 
		MenuState menuState = MenuState::NONE;
		EasingState easingState = EasingState::MOVE_IN; // �ŏ����瓮��
		// �{�^���摜
		Sprite* spStageBG1  = nullptr; // �w�i
		Sprite* cursor = nullptr; // �J�[�\��
		Sprite* buttons[MAX_BUTTON] = {}; // �{�^��
		// �����摜
		Sprite* playerGuide = nullptr; // ��l���̐���
		Sprite* enemyGuides = nullptr; // �G�̐���
		// ���̑�
		float easingCounts[MAX_BUTTON] = {}; // �{�^���̐����̃J�E���^�[
		bool menuExists = true; // ���j���[���J����Ă邩�ǂ����̏��
		bool cursorDisp = false; // �J�[�\���\��
		float maxPosY = 0; // �ő�l
		float minPosY = 0; // �ŏ��l
		Math::Vector2 prevPos = {0.0f,0.0f};
	public:
		Menu();
		~Menu();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // �����o�֐�
		void EasingMove(int i,EasingState easingState); // �C�[�W���O�ňړ�
		void CountsUpdate(); // �S�ẴJ�E���g���X�V
		void CountUpdate(int& count); // �X�̃J�E���g���X�V
		void CheckLimitPos();// �ŏ��ő�l�����߂�

		void MoveCursor(); // �J�[�\������
		void CheckMenuState(); // ���j���[�̏�Ԃ����W�Ō��߂�
	};
}