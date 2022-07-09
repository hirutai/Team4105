#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
#include <memory>

class Sprite;

namespace XIIlib {

	class GameScene;
	class Audio;

	// �X�e�[�W�I���V�[���̃N���X
	class Select : public SceneState {
	private: // �����o�萔
		const float GAP = 10.0f;
		const float HOME_POSY = 400;
		const Math::Vector2 BASE_SIZE = {320,240};
		const Math::Vector2 HOMEC_POS 
			= {1280/2,HOME_POSY };
		const Math::Vector2 HOMEL_POS 
			= {HOMEC_POS.x - BASE_SIZE.x - GAP ,HOME_POSY };
		const Math::Vector2 HOMER_POS 
			= { HOMEC_POS.x + BASE_SIZE.x + GAP ,HOME_POSY };
		Audio* audio_ = nullptr;
	private: // �����o�ϐ�
		// �ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		Math::Vector2 cursorPos = HOMEL_POS;
#pragma region Sprite�f�[�^
		std::unique_ptr<Sprite> easyButton;
		std::unique_ptr<Sprite> normalButton;
		std::unique_ptr<Sprite> hardButton;

		std::unique_ptr<Sprite> easyYankeeSilhouette;
		std::unique_ptr<Sprite> easyYankee;
		std::unique_ptr<Sprite> normalYankeeSilhouette;
		std::unique_ptr<Sprite> normalYankee;
		std::unique_ptr<Sprite> bossSilhouette;
		std::unique_ptr<Sprite> boss;

		std::unique_ptr<Sprite> yankee1;
		std::unique_ptr<Sprite> yankee2;
		
		std::unique_ptr<Sprite> edge;
		std::unique_ptr<Sprite> bg;
		std::unique_ptr<Sprite> bg_blood;
		std::unique_ptr<Sprite> stageSelect_sp;

		bool stage_easy, stage_normal, stage_boss, select;

#pragma endregion
		

	public:
		Select();
		~Select();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;

		Audio* GetAudio() { return audio_; };//��
	};
}

