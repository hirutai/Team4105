#pragma once
#include "../Input/GamePAD_XInput.h"
namespace XIIlib
{
	class GameScene;

	// �X�v���C�g�̖��O
	enum SpriteName
	{
		PUSHA_SP = 6,
		GAMEOVER_SP,
		STAGEBG1_SP,
	};

	// �X�e�[�W�i���o�[
	enum StageNumber
	{
		DEBUG = -1,
		EASY,
		NORMAL,
		HARD,
	};
	// ����ԃN���X
	class SceneState
	{
	protected:
		SceneState(){}
	public:
		// �R�s�[�֎~
		SceneState(const SceneState&) = delete;
		SceneState& operator=(const SceneState&) = delete;

		// ���z�f�X�g���N�^
		virtual ~SceneState() = default;

	public:
		virtual void Initialize(GameScene* p_game_scene) = 0;// ������
		virtual void Update(GameScene* p_game_scene) = 0;//�@�X�V
		virtual void Draw() = 0;// �`��
		virtual void DrawTex() = 0;//�X�v���C�g�`��
		virtual void DrawBackground() = 0;
	public: // Setter�֐�
		void SetGamePad(GamePAD_XInput* gamePad) { this->gamePad_ = gamePad; } // GamePad�̐ݒ�
	protected: // �ÓI�����o�ϐ�
		static StageNumber stageNum; // �X�e�[�W�i���o�[
		// GamePad�̎؂蕨�ϐ�
		static GamePAD_XInput* gamePad_;
	};
}