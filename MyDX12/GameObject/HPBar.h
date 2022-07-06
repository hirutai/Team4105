#pragma once

#include "../Struct/Math.h"
#include"BossHP.h"

namespace XIIlib
{
	class BillObj;

	/// <summary>
	/// �G�̍U���^�C�}�[
	/// </summary>
	class HPBar
	{
#pragma region �G�C���A�X
	private:
		using Vector2 = Math::Vector2;
		using Vector3 = Math::Vector3;
#pragma endregion

#pragma region �萔
		float countingNum = BossHP::GetInstance()->GetBossHP(); // ��������HP

		const float maxSize = 30.0f; // �ő�T�C�Y
#pragma endregion

#pragma region �����o�֐�
	public:
		HPBar(const HPBar& obj) = delete;
		HPBar& operator = (const HPBar& obj) = delete;

		static HPBar* GetInstance();
		// �R���X�g���N�^
		HPBar();
		// ������
		void Initialize();
		// �^�C�}�[�֐�
		void Timer(float attackPoint);
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

		Vector2 barSize{ 30.0f, 0.75f *5}; // �o�[�̃T�C�Y
		Vector2 edgeSize{ 30.5f, 1.0f *5}; // ���̃T�C�Y

		float sumPoint;
#pragma endregion
	};
}