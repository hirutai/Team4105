#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

namespace XIIlib {

	class KeyInput {
	private:
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // 静的メンバ変数
		static KeyInput* GetInstance();

	public: // メンバ関数

		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		void Update();

		bool Push(BYTE keyNumber);

		bool Trigger(BYTE keyNumber);

	private: // メンバ関数
		KeyInput() = default;
		KeyInput(const KeyInput&) = delete;
		~KeyInput() = default;
		KeyInput& operator=(const KeyInput&) = delete;

	private: // メンバ変数
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devkeyboard;
		BYTE key[256] = {};
		BYTE keyPre[256] = {};
	};

}