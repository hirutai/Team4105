#pragma once
#include "SceneState.h"

namespace XIIlib {

	class GameScene;
	class IntervalTimer;

	// �v���C�V�[���̃N���X
	class Play : public SceneState {
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		IntervalTimer* intervalTimter = nullptr; // �G�̍s���Ԋu�p�^�C�}�[
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
	};
}