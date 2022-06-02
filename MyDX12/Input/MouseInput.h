#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
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
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // �ÓI�����o�ϐ�
		static MouseInput* GetInstance();

	public: // �����o�֐�

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

	private: // �����o�֐�
		MouseInput() = default;
		MouseInput(const MouseInput&) = delete;
		~MouseInput() = default;
		MouseInput& operator=(const MouseInput&) = delete;

	private: // �����o�ϐ�
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devMouse;
		DIMOUSESTATE2 mouseState = {};
		DIMOUSESTATE2 mouseStatePre = {};
		POINT p;

	};

}