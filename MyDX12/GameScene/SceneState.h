#pragma once

namespace XIIlib
{
	class GameScene;

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
	};
}