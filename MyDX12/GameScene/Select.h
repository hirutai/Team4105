#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>

class Sprite;

namespace XIIlib {

	class GameScene;

	// �X�e�[�W�I���V�[���̃N���X
	class Select : public SceneState {
	private: // �����o�萔
		const float homePosY = 0;
		const Math::Vector2 HOMEL_POS = {};
		const Math::Vector2 HOMEC_POS = {};
		const Math::Vector2 HOMER_POS = {};
	private: // �����o�ϐ�
		// �ϐ��錾�̏�(Object/Sprite/����/�t���O��)
#pragma region Sprite�f�[�^
		std::unique_ptr<Sprite> easyButton;
		std::unique_ptr<Sprite> normalButton;
		std::unique_ptr<Sprite> hardButton;
		std::unique_ptr<Sprite> edge;
#pragma endregion
		

	public:
		Select();
		~Select();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	};
}

