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
		const float GAP = 100.0f;
		const float HOME_POSY = 250;
		const Math::Vector2 BASE_SIZE = {360,240};
		const Math::Vector2 HOMEL_POS 
			= {0,HOME_POSY };
		const Math::Vector2 HOMEC_POS 
			= {HOMEL_POS.x + BASE_SIZE.x + GAP ,HOME_POSY };
		const Math::Vector2 HOMER_POS 
			= { HOMEC_POS.x + BASE_SIZE.x + GAP ,HOME_POSY };
	private: // �����o�ϐ�
		// �ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		Math::Vector2 edgePos = HOMEL_POS;
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

