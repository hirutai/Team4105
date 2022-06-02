#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

namespace Math {
	struct Vector3;
}

namespace XIIlib {

	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	struct Mouse
	{
		float m_X;
		float m_Y;
		float m_Z;
	};

	class MouseInput {
	private:
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // 静的メンバ変数
		static MouseInput* GetInstance();

	public: // メンバ関数

		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		void Update();

		bool PushMouseLeft();

		bool PushMouseRight();

		bool PushMouseMiddle();

		bool TriggerMouseLeft();

		bool TriggerMouseRight();

		bool TriggerMouseMiddle();

		MouseMove GetMouseMove();

		Math::Vector3 GetScreenMousePos();
		Math::Vector3 GetDesktopMousePos();

	private: // メンバ関数
		MouseInput() = default;
		MouseInput(const MouseInput&) = delete;
		~MouseInput() = default;
		MouseInput& operator=(const MouseInput&) = delete;

	private: // メンバ変数
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devMouse;
		DIMOUSESTATE2 mouseState = {};
		DIMOUSESTATE2 mouseStatePre = {};
		POINT p;

	};

}