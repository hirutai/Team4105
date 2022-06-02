#include "MouseInput.h"
#include "../Struct/Math.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace XIIlib;
using namespace Math;

MouseInput* MouseInput::GetInstance()
{
	static MouseInput instance;

	return &instance;
}

bool MouseInput::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	return true;
}

void MouseInput::Update()
{
	HRESULT result;

	{// マウス
		result = devMouse->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;

		GetCursorPos(&p);

		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}
}

bool MouseInput::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[0])
	{
		return true;
	}
	// 押していない
	return false;
}

bool MouseInput::PushMouseRight()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[1])
	{
		return true;
	}
	// 押していない
	return false;
}

bool MouseInput::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[2])
	{
		return true;
	}
	// 押していない
	return false;
}

bool MouseInput::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}
	// トリガーでない
	return false;
}

bool MouseInput::TriggerMouseRight()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1])
	{
		return true;
	}
	// トリガーでない
	return false;
}

bool MouseInput::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}
	// トリガーでない
	return false;
}

XIIlib::MouseMove MouseInput::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

Math::Vector3 MouseInput::GetScreenMousePos()
{
	Math::Vector3 point;
	ScreenToClient(FindWindowA("DirectXWindows", nullptr), &p);
	point.x = p.x;
	point.y = p.y;
	point.z = 0.0f;
	return point;
}

Math::Vector3 MouseInput::GetDesktopMousePos()
{
	Math::Vector3 point;
	point.x = p.x;
	point.y = p.y;
	point.z = 0.0f;
	return point;
}