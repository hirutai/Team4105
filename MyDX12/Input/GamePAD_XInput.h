#pragma once
#pragma comment (lib, "xinput.lib")
#include <Windows.h>
#include <Xinput.h>

// ���O�ȗ���
enum XinputButtonMask
{
	X_UP = XINPUT_GAMEPAD_DPAD_UP,          // 0x0001 
	X_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,      // 0x0002
	X_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,      // 0x0004
	X_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,    // 0x0008
	X_START = XINPUT_GAMEPAD_START,         // 0x0010
	X_BACK = XINPUT_GAMEPAD_BACK,           // 0x0020 
	X_THUMBL = XINPUT_GAMEPAD_LEFT_THUMB,   // 0x0040 L�X�e�B�b�N��������
	X_THUMBR = XINPUT_GAMEPAD_RIGHT_THUMB,  // 0x0080 R�X�e�B�b�N��������
	X_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,    // 0x0100
	X_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,   // 0x0200 
	X_A = XINPUT_GAMEPAD_A,                 // 0x1000 A�{�^��
	X_B = XINPUT_GAMEPAD_B,                 // 0x2000 B�{�^��
	X_X = XINPUT_GAMEPAD_X,                 // 0x4000 X�{�^��
	X_Y = XINPUT_GAMEPAD_Y,                 // 0x8000 Y�{�^��
};

class GamePAD_XInput
{
public:
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(int gamePadNum = 0);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

#pragma region �{�^���S��

	/// <summary>
	/// �{�^���������Ă��
	/// </summary>
	/// <param name="buttonNumber">�m�F�������{�^���̔ԍ�</param>
	/// <returns>����</returns>
	bool PressButton(int buttonNumber);

	/// <summary>
	/// �{�^�����������u��
	/// </summary>
	/// <param name="buttonNumber">�m�F�������{�^���̔ԍ�</param>
	/// <returns>����</returns>
	bool Button_Down(int buttonNumber);

	/// <summary>
	/// �{�^���𗣂����u��
	/// </summary>
	/// <param name="buttonNumber">�m�F�������{�^���̔ԍ�</param>
	/// <returns>����</returns>
	bool Button_Up(int buttonNumber);

	/// <summary>
	/// ���炩�̃{�^����������Ă��邩
	/// </summary>
	/// <returns>����</returns>
	bool PressButtonAll();

#pragma endregion

#pragma region �E�X�e�B�b�N

	// �E�X�e�B�b�N���|��Ă��邩
	bool RStickDown();
	// �E�X�e�B�b�N��|�����u�ԁ@�O
	bool RStickDownFront();
	// �E�X�e�B�b�N��|�����u�ԁ@��
	bool RStickDownBack();
	// �E�X�e�B�b�N��|�����u�ԁ@��
	bool RStickDownLeft();
	// �E�X�e�B�b�N��|�����u�ԁ@�E
	bool RStickDownRight();
	// �E�X�e�B�b�N�̉����@-1.0f�`1.0f ��Ԃ��@�� < 0 < �E
	float RStickXaxis();
	// �E�X�e�B�b�N�̏c���@-1.0f�`1.0f ��Ԃ��@�� < 0 < ��
	float RStickYaxis();

#pragma endregion

#pragma region ���X�e�B�b�N

	// ���X�e�B�b�N���|��Ă��邩
	bool LStickDown();
	// ���X�e�B�b�N��|�����u�ԁ@�O
	bool LStickDownFront();
	// ���X�e�B�b�N��|�����u�ԁ@��
	bool LStickDownBack();
	// ���X�e�B�b�N��|�����u�ԁ@��
	bool LStickDownLeft();
	// ���X�e�B�b�N��|�����u�ԁ@�E
	bool LStickDownRight();

	// ���X�e�B�b�N�̉����@-1.0f�`1.0f ��Ԃ��@�� < 0 < �E
	float LStickXaxis();
	// ���X�e�B�b�N�̏c���@-1.0f�`1.0f ��Ԃ��@�� < 0 < ��
	float LStickYaxis();

#pragma endregion

#pragma region �E�g���K�[

	// �E�g���K�[(LT)�����Ă����
	bool TriggerRPress();
	// �E�g���K�[(RT)�������u��
	bool TriggerRDown();
	// �E�g���K�[(RT)�������u��
	bool TriggerRUp();
	// �E�g���K�[(RT)�̉������݁@0�`1.0f ��Ԃ�
	float TriggerRDegree();

#pragma endregion

#pragma region ���g���K�[

	// ���g���K�[(LT)�����Ă����
	bool TriggerLPress();
	// ���g���K�[(LT)�������u��
	bool TriggerLDown();
	// ���g���K�[(LT)�������u��
	bool TriggerLUp();
	// ���g���K�[(LT)�̉������݁@0�`1.0f ��Ԃ�
	float TriggerLDegree();

#pragma endregion

#pragma region �U��

	// �U��
	void Vibration();
	// �U�� �E����
	void VibrationR();
	// �U�� ������
	void VibrationL();
	// �U�����~�߂�
	void VibrationReset();

	// �U���̑傫���ύX
	inline void SetVibrationLevel(int vibLv) { GamePAD_XInput::vibrationLevel = vibLv; }

#pragma endregion

private:
#pragma region �������u�Ԃ̔���p�֐�

	float PreStickRX();
	float PreStickRY();
	float PreStickLX();
	float PreStickLY();
	float PreTriggerR();
	float PreTriggerL();

#pragma endregion

private: // �����o�ϐ�
	// �\���̂̃C���X�^���X
	XINPUT_STATE XinputState;
	// �O�t���[���̏�Ԏ擾
	XINPUT_STATE preXinputState;
	// �U���̑傫�� 0�`65535(2�O16)
	static int vibrationLevel;
};




