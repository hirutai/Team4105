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

	// DirectInput�I�u�W�F�N�g�̐���
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��

	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	return true;
}

void MouseInput::Update()
{
	HRESULT result;

	{// �}�E�X
		result = devMouse->Acquire();	// �}�E�X����J�n

		// �O��̓��͂�ۑ�
		mouseStatePre = mouseState;

		GetCursorPos(&p);

		// �}�E�X�̓���
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}
}

bool MouseInput::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool MouseInput::PushMouseRight()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[1])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool MouseInput::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool MouseInput::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}
	// �g���K�[�łȂ�
	return false;
}

bool MouseInput::TriggerMouseRight()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1])
	{
		return true;
	}
	// �g���K�[�łȂ�
	return false;
}

bool MouseInput::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}
	// �g���K�[�łȂ�
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