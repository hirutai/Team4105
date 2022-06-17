#pragma once
// �v���g�^�C�v�錾
class WindowsApp;
class DirectX12;
class Sound;

namespace XIIlib
{
	class KeyInput;
	class ControllerInput;

	class Library
	{
	private:
		WindowsApp* w_app;
		DirectX12* dx12;
		KeyInput* keyInput;
		//ControllerInput* pad_input;
		const unsigned int timerId = 1;
		const unsigned int frameRate = (1000 / 60);

	public:
		// �R���X�g���N�^
		Library();
		// �f�X�g���N�^
		~Library();
		// ������
		int Initialize();
		// FPS�̌Œ�(�Ȉ�)
		void SetFPS();
		// Timer�̔j��
		void ReleaseTimer();
		// ���b�Z�[�W
		bool Message();
		// ���͏����̍X�V
		void InputUpdate();
		// �`��J�n
		void D_Start();
		// �`��I��
		void D_End();
	};
}