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

	// DirectInput�I�u�W�F�N�g�̐���
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// �L�[�{�[�h�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	// ���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��

	// �r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��

	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	if (devJoystick != nullptr)
	{
		// �R���g���[���[�f�o�C�X�̐���	
		result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);

		// ���̓f�[�^�`���̃Z�b�g
		result = devJoystick->SetDataFormat(&c_dfDIJoystick); // �W���`��

		// �r�����䃌�x���̃Z�b�g
		result = devJoystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

		// �f�o�C�X�̔\�͂��擾
		result = devJoystick->GetCapabilities(&diDevCaps);
	}

	//result = devJoystick->EnumObjects(EnumAxesCallback, (void*)hwnd, DIDFT_AXIS);

	return true;
}

void Input::Update()
{
	HRESULT result;

	{// �L�[�{�[�h
		result = devkeyboard->Acquire();	// �L�[�{�[�h����J�n

		// �O��̃L�[���͂�ۑ�
		memcpy(keyPre, key, sizeof(key));

		// �L�[�̓���
		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	{// �}�E�X
		result = devMouse->Acquire();	// �}�E�X����J�n

		// �O��̓��͂�ۑ�
		mouseStatePre = mouseState;

		GetCursorPos(&p);

		// �}�E�X�̓���
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	{// �W���C�X�e�B�b�N(�R���g���[���[)
		if (devJoystick != nullptr)
		{
			result = devJoystick->Acquire();

			// �O��̓��͏���ۑ�
			joyStatePre = joyState;

			// �W���C�X�e�B�b�N�̓���
			result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);
		}
	}
}

bool Input::Push(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key[keyNumber])
	{return true;}

	// �����Ă��Ȃ�
	return false;
}

bool Input::Trigger(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber])
	{return true;}

	// �g���K�[�łȂ�
	return false;
}

bool Input::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseRight()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[1])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2])
	{
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}
	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseRight()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1])
	{
		return true;
	}
	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}
	// �g���K�[�łȂ�
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

	inp.cS = joyState.rgdwPOV[0];// 0��,90��,180��,270��
	return inp;
}

bool Input::PushJoyStick(int index)
{
	// 0�łȂ���Ή����Ă���
	if (joyState.rgbButtons[index] & 0x80)
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerJoyStick(int index)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!joyStatePre.rgbButtons[index] && joyState.rgbButtons[index])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}