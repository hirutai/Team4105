#pragma once
#include "SceneState.h"
class Sprite;

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
namespace XIIlib
{
	class GameScene;

	class Clear : public SceneState
	{
	public: // �����o�֐�
		// �R���X�g���N�^
		Clear();
		// �f�X�X�g���N�^
		~Clear();

		// ������
		void Initialize(GameScene* p_game_scene)override;
		// �X�V
		void Update(GameScene* p_game_scene)override;
		// �`��
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // �����o�ϐ�
		Sprite* gameClear = nullptr; // �N���A�摜
		bool oneThrough = false;
	};

}