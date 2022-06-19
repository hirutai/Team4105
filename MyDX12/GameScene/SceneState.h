#pragma once
#include "../Input/GamePAD_XInput.h"
namespace XIIlib
{
	class GameScene;

	// �X�v���C�g�̖��O
	enum class SpriteName : int
	{
		PUSHA_SP = 6,
		GAMEOVER_SP,
		STAGEBG1_SP,
		EASY_BUTTON_SP,
		NORMAL_BUTTON_SP,
		HARD_BUTTON_SP,
		EDGE_SP,
		TITLEBG_SP,
		TITLELOG_SP,
		GAMECLEAR_SP,
		PLAYERGUIDE_SP,
		MENU_SP,
		ENEMYGUIDES_SP,
	};

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