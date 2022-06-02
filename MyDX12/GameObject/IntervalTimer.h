#pragma once

class DigitalNumberText;

// ���W
#define POSX 0
#define POSY 0
// �傫��
#define SCALE 1.0f

// �����J�E���^�[
//#define INITIALCOUNTER 60 + 60 * MAXNUM // �����J�E���g��
#define INITIALCOUNTER 60 + 60 * MAXNUM // �����J�E���g��
#define ENDCOUNTER 0 // �I���J�E���g��

// �\���J�E���^�[
#define MAXNUM DISPLAYTWO
#define DISPLAYTWO 2
#define DISPLAYONE (DISPLAYTWO - 1)
#define DISPLAYZERO (DISPLAYONE - 1)

namespace XIIlib
{
/// <summary>
/// �G�̍s���Ԋu�p�^�C�}�[
/// </summary>
	class IntervalTimer
	{
	public: // �����o�֐�
		void Initialize(int textureNumber); // ������

		void Timer(); // �^�C�}�[�֐�

		void Draw(); // �`��

		const unsigned int GetTimer() { return internalCounter; } // �^�C�}�[�̎擾
		const unsigned int GetGameTimer() { return displayCounter; } // �^�C�}�[�̎擾

	private: // �����o�ϐ�
		unsigned int internalCounter = INITIALCOUNTER; // �����J�E���^�[

		unsigned int displayCounter = MAXNUM; // �\���J�E���^�[

		DigitalNumberText* timerNum = nullptr; // �^�C�}�[�p�̐����摜
	};
}
