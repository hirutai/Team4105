#include "Input.h"
#include "../Struct/Math.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
using namespace Math;

const float Input::StickHalf = 16383.5f;
const float Input::StickCount = 32767.0f;
const float Input::StickMax = 65535.0f;

Input * Input::GetInstance()
{
	static Input instance;

	return &instance;
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// キーボードデバイスの生成	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	// 入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

	// 排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	if (devJoystick != nullptr)
	{
		// コントローラーデバイスの生成	
		result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);

		// 入力データ形式のセット
		result = devJoystick->SetDataFormat(&c_dfDIJoystick); // 標準形式

		// 排他制御レベルのセット
		result = devJoystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

		// デバイスの能力を取得
		result = devJoystick->GetCapabilities(&diDevCaps);
	}

	//result = devJoystick->EnumObjects(EnumAxesCallback, (void*)hwnd, DIDFT_AXIS);

	return true;
}

void Input::Update()
{
	HRESULT result;

	{// キーボード
		result = devkeyboard->Acquire();	// キーボード動作開始

		// 前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		// キーの入力
		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	{// マウス
		result = devMouse->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;

		GetCursorPos(&p);

		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	{// ジョイスティック(コントローラー)
		if (devJoystick != nullptr)
		{
			result = devJoystick->Acquire();

			// 前回の入力情報を保存
			joyStatePre = joyState;

			// ジョイスティックの入力
			result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);
		}
	}
}

bool Input::Push(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (key[keyNumber])
	{return true;}

	// 押していない
	return false;
}

bool Input::Trigger(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre[keyNumber] && key[keyNumber])
	{return true;}

	// トリガーでない
	return false;
}

bool Input::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[0])
	{
		return true;
	}
	// 押していない
	return false;
}

bool Input::PushMouseRight()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[1])
	{
		return true;
	}
	// 押していない
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[2])
	{
		return true;
	}
	// 押していない
	return false;
}

bool Input::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}
	// トリガーでない
	return false;
}

bool Input::TriggerMouseRight()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1])
	{
		return true;
	}
	// トリガーでない
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}
	// トリガーでない
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

Math::Vector3 Input::GetScreenMousePos()
{
	Math::Vector3 point;
	ScreenToClient(FindWindowA("DirectXWindows", nullptr), &p);
	point.x = p.x;
	point.y = p.y;
	point.z = 0.0f;
	return point;
}

Math::Vector3 Input::GetDesktopMousePos()
{
	Math::Vector3 point;
	point.x = p.x;
	point.y = p.y;
	point.z = 0.0f;
	return point;
}

Input::JoystickInput Input::GetJoystickData()
{
	JoystickInput inp;
	inp.lX = joyState.lX;
	inp.lY = joyState.lY;
	inp.rX = joyState.lZ;
	inp.rY = joyState.lRz;

	inp.cS = joyState.rgdwPOV[0];// 0°,90°,180°,270°
	return inp;
}

bool Input::PushJoyStick(int index)
{
	// 0でなければ押している
	if (joyState.rgbButtons[index] & 0x80)
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerJoyStick(int index)
{
	// 前回が0で、今回が0でなければトリガー
	if (!joyStatePre.rgbButtons[index] && joyState.rgbButtons[index])
	{
		return true;
	}

	// トリガーでない
	return false;
}