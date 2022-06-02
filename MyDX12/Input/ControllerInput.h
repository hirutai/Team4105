#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
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
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // 静的メンバ変数
		static ControllerInput* GetInstance();

	public: // メンバ関数

		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		void Update();

		JoystickInput GetJoystickData();

		bool PushJoyStick(int index);
		bool TriggerJoyStick(int index);

		void Cleanup();

		static const float StickHalf;
		static const float StickCount;
		static const float StickMax;

	private: // メンバ関数
		ControllerInput() = default;
		ControllerInput(const ControllerInput&) = delete;
		~ControllerInput() = default;
		ControllerInput& operator=(const ControllerInput&) = delete;

	private: // メンバ変数
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devJoystick;
		DIJOYSTATE joyState = {};
		DIJOYSTATE joyStatePre = {};
		DIDEVCAPS diDevCaps;

	};
}