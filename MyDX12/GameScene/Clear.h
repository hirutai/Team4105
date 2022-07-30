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
	private:
		const float yMullValue1 = 0.5f, yMullValue2 = 0.7f,
			xMullValue = 0.53f, mulXY = 1.5f, pickSize = 1.5f;
		const float xMullValueHard = 0.3f;
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

		Sprite* textSelect = nullptr;
		Sprite* textTitle = nullptr;
		ChainSprite* pchStr = nullptr;
		bool selectT_R = false;
	};

}