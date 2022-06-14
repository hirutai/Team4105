#include "GamePAD_XInput.h"

int GamePAD_XInput::vibrationLevel = 65535;

bool GamePAD_XInput::Initialize(int gamePadNum)
{
	DWORD result;

	result = XInputGetState(
		gamePadNum, // コントローラー番号の指定(0～3)
		&XinputState); // 格納先の XINPUT_STATE 構造体のポインタを渡す

	// 繋がらなかった
	if (result != ERROR_SUCCESS)return false;

	// 繋がった
	return true;
}

void GamePAD_XInput::Update()
{
	// 1f前の入力を保存
	preXinputState = XinputState;
	// コントローラーの状態を取得
	XInputGetState(
		0,		// コントローラー番号の指定(0～3)
		&XinputState);// 格納先の XINPUT_STATE 構造体のポインタを渡す
}

bool GamePAD_XInput::PressButton(int buttonNumber)
{
	if (XinputState.Gamepad.wButtons & buttonNumber) return true;
	return false;
}

bool GamePAD_XInput::Button_Down(int buttonNumber)
{
	if (XinputState.Gamepad.wButtons & buttonNumber &&
		(preXinputState.Gamepad.wButtons & buttonNumber) == 0) return true;
	return false;
}

bool GamePAD_XInput::Button_Up(int buttonNumber)
{
	if ((XinputState.Gamepad.wButtons & buttonNumber) == 0 &&
		preXinputState.Gamepad.wButtons & buttonNumber) return true;
	return false;
}

bool GamePAD_XInput::PressButtonAll()
{
	if (XinputState.Gamepad.wButtons == 0)return false;
	return true;
}

bool GamePAD_XInput::RStickDown()
{
	if (RStickYaxis() != 0 ||
		RStickXaxis() != 0) return true;
	return false;
}

bool GamePAD_XInput::RStickDownFront()
{
	if (RStickYaxis() > 0 &&
		PreStickRY() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::RStickDownBack()
{
	if (RStickYaxis() < 0 &&
		PreStickRY() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::RStickDownLeft()
{
	if (RStickXaxis() < 0 &&
		PreStickRX() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::RStickDownRight()
{
	if (RStickXaxis() > 0 &&
		PreStickRX() == 0.0f) return true;
	return false;
}

float GamePAD_XInput::RStickXaxis()
{
	float result;
	result = (float)XinputState.Gamepad.sThumbRX / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

float GamePAD_XInput::RStickYaxis()
{
	float result;
	result = (float)XinputState.Gamepad.sThumbRY / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

bool GamePAD_XInput::LStickDown()
{
	if (LStickYaxis() != 0 ||
		LStickXaxis() != 0) return true;
	return false;
}

bool GamePAD_XInput::LStickDownFront()
{
	if (LStickYaxis() > 0 &&
		PreStickLY() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::LStickDownBack()
{
	if (LStickYaxis() < 0 &&
		PreStickLY() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::LStickDownLeft()
{
	if (LStickXaxis() < 0 &&
		PreStickLX() == 0.0f) return true;
	return false;
}

bool GamePAD_XInput::LStickDownRight()
{
	if (LStickXaxis() > 0 &&
		PreStickLX() == 0.0f) return true;
	return false;
}

float GamePAD_XInput::LStickXaxis()
{
	float result;
	result = (float)XinputState.Gamepad.sThumbLX / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

float GamePAD_XInput::LStickYaxis()
{
	float result;
	result = (float)XinputState.Gamepad.sThumbLY / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

bool GamePAD_XInput::TriggerRPress()
{
	if (TriggerRDegree() > 0) return true;
	return false;
}

bool GamePAD_XInput::TriggerRDown()
{
	if (TriggerRDegree() > 0 &&
		PreTriggerR() == 0) return true;
	return false;
}

bool GamePAD_XInput::TriggerRUp()
{
	if (TriggerRDegree() == 0 &&
		PreTriggerR() > 0) return true;
	return false;
}

float GamePAD_XInput::TriggerRDegree()
{
	float result;
	result = (float)XinputState.Gamepad.bRightTrigger / 255;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

bool GamePAD_XInput::TriggerLPress()
{
	if (TriggerLDegree() > 0) return true;
	return false;
}

bool GamePAD_XInput::TriggerLDown()
{
	if (TriggerLDegree() > 0 &&
		PreTriggerL() == 0) return true;
	return false;
}

bool GamePAD_XInput::TriggerLUp()
{
	if (TriggerLDegree() == 0 &&
		PreTriggerL() > 0) return true;
	return false;
}

float GamePAD_XInput::TriggerLDegree()
{
	float result;
	result = (float)XinputState.Gamepad.bLeftTrigger / 255;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

void GamePAD_XInput::Vibration()
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = vibrationLevel;
	vibration.wRightMotorSpeed = vibrationLevel;
	XInputSetState(0, &vibration);
}

void GamePAD_XInput::VibrationR()
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = vibrationLevel;
	XInputSetState(0, &vibration);
}

void GamePAD_XInput::VibrationL()
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = vibrationLevel;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);
}

void GamePAD_XInput::VibrationReset()
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);
}

float GamePAD_XInput::PreStickRX()
{
	float result;
	result = (float)preXinputState.Gamepad.sThumbRX / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return 0.0f;
}

float GamePAD_XInput::PreStickRY()
{
	float result;
	result = (float)XinputState.Gamepad.sThumbRY / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

float GamePAD_XInput::PreStickLX()
{
	float result;
	result = (float)preXinputState.Gamepad.sThumbLX / 32767;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}

float GamePAD_XInput::PreStickLY()
{
	float result;
	result = (float)preXinputState.Gamepad.bLeftTrigger / 255;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}


float GamePAD_XInput::PreTriggerR()
{
	return 0.0f;
}

float GamePAD_XInput::PreTriggerL()
{
	float result;
	result = (float)preXinputState.Gamepad.bRightTrigger / 255;
	if (result < 0.3f && result > -0.3f) return 0;
	if (result > 1.0f) return 1.0f;
	if (result < -1.0f) return -1.0f;
	return result;
}
