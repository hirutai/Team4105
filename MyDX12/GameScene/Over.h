#pragma once
#include "SceneState.h"
class Sprite;
namespace XIIlib {

	class GameScene;
	

	// �v���C�V�[���̃N���X
	class Over : public SceneState {
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)

	private:
		const float yMullValue1 = 0.5f, yMullValue2 = 0.7f,
			xMullValue = 0.53f, mulXY = 1.5f, pickSize = 1.5f;
	public:
		Over();
		~Over();

		void Initialize()override;
		void Update()override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // �����o�ϐ�
		Sprite* spGameOver = nullptr;
		bool oneThrough = false;

		Sprite* textRetry = nullptr;
		Sprite* textTitle = nullptr;
		bool selectT_R = false;
	};
}