#pragma once
#include "SceneState.h"
class Sprite;

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
namespace XIIlib
{
	class GameScene;
	class ChainSprite;

	class Clear : public SceneState
	{
	public: // �����o�֐�
		// �R���X�g���N�^
		Clear();
		// �f�X�X�g���N�^
		~Clear();

		// ������
		void Initialize()override;
		// �X�V
		void Update()override;
		// �`��
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

	private: // �����o�ϐ�
		Sprite* gameClear = nullptr; // �N���A�摜
		bool oneThrough = false;

		Sprite* space = nullptr;
		ChainSprite* pchStr = nullptr;
	};

}