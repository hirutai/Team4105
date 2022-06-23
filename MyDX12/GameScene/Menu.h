#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
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
	// ���j���[�V�[���̃N���X
	class Menu : public SceneState {
	private: // �����o�萔
		const float MAX_EASING_COUNT = 20.0f; // Easing�̃}�b�N�X�J�E���g
		const float SPACE = 70.0f; // �Ԋu
		const int  EASING_SPACE = 3; // �Ԋu
		const Math::Vector2 winSize = { 1280.0f,768.0f }; // ��ʃT�C�Y
		const Math::Vector2 winCenter = { winSize.x/2,winSize.y / 2 }; // ��ʂ̒���
	public:
		static const int MAX_BUTTON = 5; // �X�v���C�gButton�̌�

	private:
		// ���j���[���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		MenuState menuState = MenuState::NONE;
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

	public:
		Menu();
		~Menu();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private:
		void EasingMove(int i); // �C�[�W���O�ňړ�
		void CountUpdate(int& count); // �J�E���^�[���X�V
		void CountsUpdate(); // �S�ẴJ�E���g���X�V
	};
}