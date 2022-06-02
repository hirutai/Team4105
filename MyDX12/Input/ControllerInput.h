#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

namespace XIIlib {

	enum struct Button {
		X,Y,A,B,
		LB,RB,
		LT,RT,
		LS,RS,
		BACK,
		START,
		GUIDE
	};

	struct JoystickInput
	{
		LONG lX;
		LONG lY;
		LONG rX;
		LONG rY;

		DWORD cS;
	};

	class ControllerInput {
	private:
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // �ÓI�����o�ϐ�
		static ControllerInput* GetInstance();

	public: // �����o�֐�

		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		void Update();

		JoystickInput GetJoystickData();

		bool PushJoyStick(int index);
		bool TriggerJoyStick(int index);

		void Cleanup();

		static const float StickHalf;
		static const float StickCount;
		static const float StickMax;

	private: // �����o�֐�
		ControllerInput() = default;
		ControllerInput(const ControllerInput&) = delete;
		~ControllerInput() = default;
		ControllerInput& operator=(const ControllerInput&) = delete;

	private: // �����o�ϐ�
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devJoystick;
		DIJOYSTATE joyState = {};
		DIJOYSTATE joyStatePre = {};
		DIDEVCAPS diDevCaps;

	};
}