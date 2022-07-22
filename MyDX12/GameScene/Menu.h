#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>
class Sprite;

namespace XIIlib {

	class GameScene;
	// �J�[�\�����
	enum class CursorState : int
	{
		NONE = -1,
		RULE,
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
		RULE,
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
		const float SPACE = 100.0f; // �Ԋu
		const int  EASING_SPACE = 3; // �Ԋu
		const Math::Vector2 winSize = { 1280.0f,768.0f }; // ��ʃT�C�Y
		const Math::Vector2 winCenter = { winSize.x/2,winSize.y / 2 }; // ��ʂ̒���
	public:
		static const int MAX_BUTTON = 6; // �X�v���C�gButton�̌�

	private:
		// ���j���[���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		CursorState cursorState = CursorState::CONTINUE; 
		MenuState menuState = MenuState::NONE;
		EasingState easingState = EasingState::MOVE_IN; // �ŏ����瓮��
		// ���[��
		std::unique_ptr<Sprite> rule;
		// �{�^���摜
		std::unique_ptr<Sprite> spStageBG1; // �w�i
		std::unique_ptr<Sprite> cursor; // �J�[�\��
		std::unique_ptr<Sprite> buttons[MAX_BUTTON] = {}; // �{�^��
		// �����摜
		std::unique_ptr<Sprite> playerGuide; // ��l���̐���
		std::unique_ptr<Sprite> enemyGuides; // �G�̐���
		// ���̑�
		float easingCounts[MAX_BUTTON] = {}; // �{�^���̐����̃J�E���^�[
		bool menuExists = true; // ���j���[���J����Ă邩�ǂ����̏��
		bool cursorDisp = false; // �J�[�\���\��
		float maxPosY = 0; // �ő�l
		float minPosY = 0; // �ŏ��l
		Math::Vector2 prevPos = {0.0f,0.0f};
		Math::Vector2 rulePos;

		const float VEL_X = -4.0f;
		const int moveTime = 180;
		const int switchRandiMin = -3;
		const int switchRandiMax = 3;
		int count = 0;
		Math::Vector2 switchRand = { 0,0 };
	public:
		Menu();
		~Menu();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // �����o�֐�
		void EasingUpdate();
		void EasingMove(int i,EasingState easingState); // �C�[�W���O�ňړ�
		void CountsUpdate(); // �S�ẴJ�E���g���X�V
		void CountUpdate(int& count); // �X�̃J�E���g���X�V
		void CheckLimitPos();// �ŏ��ő�l�����߂�

		void MoveCursor(); // �J�[�\������
		void CheckMenuState(); // ���j���[�̏�Ԃ����W�Ō��߂�
	};
}