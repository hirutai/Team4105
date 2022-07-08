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
		const float maxSize = 60.0f; // �ő�T�C�Y
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
		void Timer(float damage);
		// �`��
		void Draw();
		// �T�C�Y�̎擾
		bool SizeZeroFlag();
		bool SizeThirdFlag();//�O���̈�
		bool SizeThirdBelowFlag();//�O���̈�ȉ�
		// �o�[�̍��W�̐ݒ�
		void SetPosition(const Vector3& position);
#pragma endregion

#pragma region �����o�ϐ�
	private:
		float decNum; // ���炷��

		float currentSize; // ���݂̃T�C�Y

		BillObj* timerBar = nullptr; // �^�C�}�[�o�[�̃I�u�W�F�N�g
		BillObj* timerEdge = nullptr; // �^�C�}�[�̉��̃I�u�W�F�N�g

		Vector2 barSize{ maxSize, 1.0f }; // �o�[�̃T�C�Y
		Vector2 edgeSize{ maxSize + 1.0f, 2.0f }; // ���̃T�C�Y

		const Vector3 hpBarPos{ 0, 0, -25 }; // HP�o�[�̍��W
#pragma endregion
	};
}