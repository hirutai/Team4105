#pragma once
#include "SceneState.h"

namespace XIIlib {

	class GameScene;

	// ���j���[�V�[���̃N���X
	class Menu : public SceneState {
	private:
		// ���j���[���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)

	public:
		Menu();
		~Menu();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private:

	};
}