#pragma once
#include <windows.h>

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
		const float MIN_FREAM_TIME = 1.0f / 60.0f;
		float frameTime = 0;
		LARGE_INTEGER timeStart;
		LARGE_INTEGER timeEnd;
		LARGE_INTEGER timeFreq;
		// fps���擾����Ȃ�0�ŏ��������Ȃ��ƃS�~���������ă}�C�i�X����n�܂����肷��(�����m��Ȃ�)
		float fps = 0;

	public:
		// �R���X�g���N�^
		Library();
		// �f�X�g���N�^
		~Library();
		// ������
		int Initialize();
		// FPS�̌Œ�(�Ȉ�)
		int GetQuery();
		// Timer�̔j��
		void RunTimer();
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