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
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		IntervalTimer* intervalTimter = nullptr; // �G�̍s���Ԋu�p�^�C�}�[
		Sprite* spStageBG1 = nullptr; // �w�i
		Sprite* menuButton = nullptr; // ���j���[
		Sprite* operatorGuide = nullptr; // �������
		bool menuExists = false; // ���j���[���J����Ă邩�ǂ����̏��
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};
}