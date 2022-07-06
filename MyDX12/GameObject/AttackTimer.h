#pragma once

#include "../Struct/Math.h"

namespace XIIlib
{
	class BillObj;

	enum class CountType
	{
		FRAME = 0,
		SECOND,
	};
	/// <summary>
	/// �G�̍U���^�C�}�[
	/// </summary>
	class AttackTimer
	{
#pragma region �G�C���A�X
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region �萔
		float countingNum = 1.0f; // ���������b��

		const float maxSize = 3.0f; // �ő�T�C�Y
		const float second = 60.0f; // ��b���̃t���[����
		const float baseDecNum = maxSize / second; // ��b���Ɍ��炷��
		float decNum; // ���炷��
#pragma endregion

#pragma region �����o�֐�
	public:
		// �R���X�g���N�^
		AttackTimer(int countingNum_, XIIlib::CountType type);
		// ������
		void Initialize();
		// �^�C�}�[�֐�
		void Timer();
		// �`��
		void Draw();
		// �^�C�}�[�̎擾
		const unsigned int GetTimer() { return timerNum; }
		// �T�C�Y�̎擾
		bool SizeZeroFlag();
		bool SizeThirdFlag();//�O���̈�
		bool SizeThirdBelowFlag();//�O���̈�ȉ�
		// �o�[�̍��W�̐ݒ�
		void SetPosition(const Vector3& position);
#pragma endregion

#pragma region �����o�ϐ�
	private:
		unsigned int timerNum; // �^�C�}�[

		float currentSize; // ���݂̃T�C�Y

		BillObj* timerBar = nullptr; // �^�C�}�[�o�[�̃I�u�W�F�N�g
		BillObj* timerEdge = nullptr; // �^�C�}�[�̉��̃I�u�W�F�N�g

		Vector2 barSize{ 3.0f, 0.75f }; // �o�[�̃T�C�Y
		Vector2 edgeSize{ 3.5f, 1.0f }; // ���̃T�C�Y
#pragma endregion
	};
}