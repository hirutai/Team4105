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

	public:
		// �R���X�g���N�^
		Library();
		// �f�X�g���N�^
		~Library();
		// ������
		int Initialize();
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