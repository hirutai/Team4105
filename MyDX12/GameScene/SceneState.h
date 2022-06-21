#pragma once
#include "../Input/GamePAD_XInput.h"
#include "../2D/SpriteLoader.h"

namespace XIIlib
{
	class GameScene;


	// �X�e�[�W�i���o�[
	enum class StageNumber : int
	{
		DEBUG = -1,
		EASY,
		NORMAL,
		HARD,
	};

	// ����ԃN���X
	class SceneState
	{
	protected:// �����o�ϐ�
		bool trigSpace = false;
		bool movingScene = false;

	protected:
		SceneState(){}
	public:
		// �R�s�[�֎~
		SceneState(const SceneState&) = delete;
		SceneState& operator=(const SceneState&) = delete;

		// ���z�f�X�g���N�^
		virtual ~SceneState() = default;

	public: // �p��
		virtual void Initialize(GameScene* p_game_scene) = 0;// ������
		virtual void Update(GameScene* p_game_scene) = 0;//�@�X�V
		virtual void Draw() = 0;// �`��
		virtual void DrawTex() = 0;//�X�v���C�g�`��
		virtual void DrawBackground() = 0;
		void CommonUpdate(GameScene* p_game_scene);
	protected: // ���ʏ���
		static void CreateUnitFormation();
	public: // Setter�֐�
		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; } // GamePad�̐ݒ�
	protected: // �ÓI�����o�ϐ�
		static StageNumber stageNum; // �X�e�[�W�i���o�[
		// GamePad�̎؂蕨�ϐ�
		static GamePAD_XInput* gamePad_;
	};
}