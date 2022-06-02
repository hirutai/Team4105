#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

namespace XIIlib {

	class KeyInput {
	private:
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // �ÓI�����o�ϐ�
		static KeyInput* GetInstance();

	public: // �����o�֐�

		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		void Update();

		bool Push(BYTE keyNumber);

		bool Trigger(BYTE keyNumber);

	private: // �����o�֐�
		KeyInput() = default;
		KeyInput(const KeyInput&) = delete;
		~KeyInput() = default;
		KeyInput& operator=(const KeyInput&) = delete;

	private: // �����o�ϐ�
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devkeyboard;
		BYTE key[256] = {};
		BYTE keyPre[256] = {};
	};

}