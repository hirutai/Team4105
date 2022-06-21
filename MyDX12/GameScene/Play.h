#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
	class IntervalTimer;

	// �v���C�V�[���̃N���X
	class Play : public SceneState {
	private:
		// ���j���[���
		enum class MenuState : int
		{
			NONE = 0,
			CONTINUE,
			PLAYER_GUIDS,
			ENEMY_GUIDS,
			NEXT_SLECT,
			NEXT_TITLE,
		};
		MenuState menuState = MenuState::NONE;
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		IntervalTimer* intervalTimter = nullptr; // �G�̍s���Ԋu�p�^�C�}�[

	private: // �����o�萔
		const float MAX_EASING_COUNT = 30.0f;
		const Math::Vector2 winSize = {1280.0f,768.0f};
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // �����o�ϐ��@�X�v���C�g�f�[�^
		Sprite* spStageBG1 = nullptr; // �w�i
		Sprite* menu = nullptr; // ���j���[
		Sprite* operatorGuide = nullptr; // �������
		//Sprite* playerGuide = nullptr; // ��l���̐���
		Sprite* enemyGuides = nullptr; // �G�̐���
		//Sprite* nextSelect = nullptr; // select��ʂɖ߂�
		//Sprite* nextTitle = nullptr; // title�ɖ߂�

	private: // �����o�ϐ�
		bool menuExists = false; // ���j���[���J����Ă邩�ǂ���
		float easingCount = 0;
	};
}