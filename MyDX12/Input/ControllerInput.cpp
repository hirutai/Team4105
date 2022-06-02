#include "ControllerInput.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace XIIlib;
const float ControllerInput::StickHalf = 16383.5f;
const float ControllerInput::StickCount = 32767.0f;
const float ControllerInput::StickMax = 65535.0f;

ControllerInput* ControllerInput::GetInstance()
{
	static ControllerInput instance;

	return &instance;
}

void XIIlib::ControllerInput::Cleanup()
{
	if (dinput.Get()) {
		if (devJoystick.Get()) {
			devJoystick->Unacquire(); // 入力デバイスへのアクセス権の解放
			devJoystick->Release(); // キーボードデバイスインターフェイスの解放
		}
		dinput->Release(); // DirectInputオブジェクトの解放
	}
}

bool ControllerInput::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(result);

	// コントローラーデバイスの生成	
	result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);
	assert(result);

	// 入力データ形式のセット
	result = devJoystick->SetDataFormat(&c_dfDIJoystick); // 標準形式
	assert(result);

	// 排他制御レベルのセット
	result = devJoystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(result);

	// デバイスの能力を取得
	result = devJoystick->GetCapabilities(&diDevCaps);
	assert(result);

	return true;
}

void ControllerInput::Update()
{
	HRESULT result;

	result = devJoystick->Acquire();
	assert(result);

	result = devJoystick->Poll();
	assert(result);

	// 前回の入力情報を保存
	joyStatePre = joyState;

	// ジョイスティックの入力
	result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);
}

XIIlib::JoystickInput ControllerInput::GetJoystickData()
{
	JoystickInput inp;
	inp.lX = joyState.lX;
	inp.lY = joyState.lY;
	inp.rX = joyState.lZ;
	inp.rY = joyState.lRz;

	inp.cS = joyState.rgdwPOV[0];// 0°,90°,180°,270°
	return inp;
}

bool ControllerInput::PushJoyStick(int index)
{
	// 0でなければ押している
	if (joyState.rgbButtons[index] & 0x80)
	{
		return true;
	}

	// 押していない
	return false;
}

bool ControllerInput::TriggerJoyStick(int index)
{
	// 前回が0で、今回が0でなければトリガー
	if (!joyStatePre.rgbButtons[index] && joyState.rgbButtons[index])
	{
		return true;
	}

	// トリガーでない
	return false;
}