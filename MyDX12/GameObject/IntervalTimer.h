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
		const int countingNum = 5; // ������b��
		const int intCountingNum = 60 * countingNum; // ������t���[����
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
		unsigned int timerNum = intCountingNum; // �^�C�}�[

		Sprite* timer = nullptr; // �^�C�}�[�摜
		Sprite* timer_bar = nullptr; // �^�C�}�[�̃o�[�摜
#pragma endregion
	};
}