#pragma once
#include "SceneState.h"
#include "../Struct/Math.h"
class Sprite;

namespace XIIlib {

	class GameScene;
	class IntervalTimer;

	// �v���C�V�[���̃N���X
	class Play : public SceneState {
	private:
		// �v���C���Ŏg���ϐ��錾�̏�(Object/Sprite/����/�t���O��)
		IntervalTimer* intervalTimter = nullptr; // �G�̍s���Ԋu�p�^�C�}�[

	private: // �����o�萔
<<<<<<< HEAD
		const float MAX_EASING_COUNT = 30.0f;
=======
		const float MAX_EASING_COUNT = 60.0f;
>>>>>>> 4965877ae8c2e767f0ad462363b71c4f4a74aeb4
		const Math::Vector2 winSize = {1280.0f,768.0f};
	public:
		Play();
		~Play();

		void Initialize(GameScene* p_game_scene)override;
		void Update(GameScene* p_game_scene)override;
		void Draw()override;
		void DrawTex()override;
		void DrawBackground()override;
	private: // �����o�ϐ�
		Sprite* spStageBG1 = nullptr; // �w�i
		Sprite* playerGuide = nullptr; // �������
		Sprite* menu = nullptr; // ���j���[
		Sprite* enemyGuides = nullptr; // �G�̐���
		bool menuExists = false;
<<<<<<< HEAD
		bool exitFlag = false;
=======
>>>>>>> 4965877ae8c2e767f0ad462363b71c4f4a74aeb4
		//Math::Vector2 eGuidesPos = {1280.0f,0.0f};
		float easingCount = 0;
	};
}