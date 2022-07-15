#pragma once
#include "SceneState.h"
class Sprite;
namespace XIIlib {

	class GameScene;
	

	// �v���C�V�[���̃N���X
	class Over : public SceneState {
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)

	public:
		Over();
		~Over();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // �����o�ϐ�
		Sprite* spGameOver = nullptr;
		bool oneThrough = false;

		Sprite* space = nullptr;
		Sprite* textRetry = nullptr;
		Sprite* textTitle = nullptr;
		bool selectT_R = false;
	};
}