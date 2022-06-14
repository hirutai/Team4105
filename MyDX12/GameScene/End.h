#pragma once
#include "SceneState.h"
class Sprite;
namespace XIIlib {

	class GameScene;
	

	// �v���C�V�[���̃N���X
	class End : public SceneState {
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)

	public:
		End();
		~End();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
	private: // �����o�ϐ�
		Sprite* spGameOver = nullptr;
	};
}