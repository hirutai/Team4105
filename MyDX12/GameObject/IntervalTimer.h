#pragma once

#include "../Struct/Math.h"

class Sprite;

namespace XIIlib
{
	/// <summary>
	/// �G�̍s���Ԋu�p�^�C�}�[
	/// </summary>
	class IntervalTimer
	{
#pragma region �G�C���A�X
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region �萔
		const float countingNum = 7; // ���������b��
		const float intCountingNum = 5 / countingNum; // ���炷��
		const float maxSize = 300; // �ő�T�C�Y
#pragma endregion

#pragma region �����o�֐�
	public:
		void Initialize(const int& timerTexNum, const int& barTexNum); // ������

		void Timer(); // �^�C�}�[�֐�

		void Draw(); // �`��

		const unsigned int GetTimer() { return timerNum; } // �^�C�}�[�̎擾
#pragma endregion

#pragma region �����o�ϐ�
	private:
		float timerNum = 0; // �^�C�}�[

		float currentSize; // ���݂̃T�C�Y

		Sprite* timer = nullptr; // �^�C�}�[�摜
		Sprite* timer_bar = nullptr; // �^�C�}�[�̃o�[�摜
#pragma endregion
	};
}