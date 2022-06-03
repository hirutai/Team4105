#pragma once
#include <vector>

namespace XIIlib
{
	class Audio;
	class SceneState;

	class GameScene
	{
	public:
		GameScene();
		~GameScene();
		// �V�[���̐؂�ւ�
		void ChangeState(SceneState* different_state);

		// ��{�֐�(RegularFunction)
		void Initialize();
		void Update();
		void Draw();
		void DrawSprite();

		Audio* GetAudio()const;

	private:
		// ���
		SceneState* state = nullptr;
		// ��
		Audio* audio = nullptr;
		
		// ����
		int isTurn = 0;

		// �U���p�t���[���J�E���g
		int attackFrame = 0;

		// �}�X�|�C���^
		int squareX = 0;
		int squareZ = 0;

		int _timer = 0;
		int at_type = 0;

		//����
	};
}