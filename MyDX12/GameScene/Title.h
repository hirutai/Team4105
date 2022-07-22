#pragma once
#include "SceneState.h"

class Sprite;

namespace XIIlib {

	class GameScene;

	// �^�C�g���V�[���̃N���X
	class Title : public SceneState {
	private:
		// �^�C�g�����Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)

	public:
		Title();
		~Title();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private:
		Sprite* spTitleBG = nullptr;
		Sprite* spTitleLog = nullptr;

		Sprite* space = nullptr;
	};
}