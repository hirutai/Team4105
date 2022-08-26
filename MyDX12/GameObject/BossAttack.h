#pragma once
#include "Boss.h"
#include "../Struct/Math.h"

// ����̗\��A�U���ƕ\���̊֐����킹�邩��

namespace XIIlib {
	// �{�X�A�^�b�N�N���X
	class BossAttack : public Boss
	{
	private: // �V���O���g����
		BossAttack() = default;
		~BossAttack() = default;
		BossAttack(const BossAttack&) = delete;
		BossAttack& operator = (const BossAttack&) = delete;
	public:
		/// <summary>
		/// �����֐�
		/// </summary>
		/// <returns>Instance</returns>
		static BossAttack* GetInstance();

	public: // �����o�֐�
		/// <summary>
		/// Attack�G���A�\���̕ϐ��Ȃǂ�������(Attack�G���A�̏������ł͂Ȃ�)
		/// </summary>
		void InitAttackDisplay();
		/// <summary>
		/// Line���߂̊j�����̍��W�������_������
		/// </summary>
		void Target();
		/// <summary>
		/// �^�C���̕\���J�ڗp
		/// </summary>
		void DispTileDeathControl();
		/// <summary>
		/// �����_���ȃ��e�I�𐶐�(�d���Ȃ�)
		/// </summary>
		void CreateMeteorPosition();
		/// <summary>
		/// ���e�I�����̃��e�I�Əd�Ȃ��Ă��Ȃ����`�F�b�N
		/// </summary>
		bool CheckMeteorArea(const Math::Point2& meteorPos);

	public: // ------------------ �\���n ------------------
		/// <summary>
		/// �c����3��\��
		/// </summary>
		void Vertical3LineDisplay();
		/// <summary>
		/// ������3��\��
		/// </summary>
		void Horizontal3LineDisplay();
		/// <summary>
		/// 1�_ 3x3���e�I
		/// </summary>
		void OneMeteor3x3Display(const Math::Point2& kingPos);
		/// <summary>
		/// �����_���܂΂� 1x1���e�I
		/// </summary>
		void RandomMeteor1x1Display();

	public: // ------------------ �U���n ------------------
		/// <summary>
		/// �c����3��U��
		/// </summary>
		void Vertical3LineAttack();
		/// <summary>
		/// ������3��U��
		/// </summary>
		void Horizontal3LineAttack();
		/// <summary>
		/// 1�_ 3x3���e�I
		/// </summary>
		void OneMeteor3x3Attack(const Math::Point2& kingPos);
		/// <summary>
		/// �����_���܂΂� 1x1���e�I
		/// </summary>
		void RandomMeteor1x1Attack();

	public: // �\���U������
		// �ߋ����ɂ���player�𐁂���΂�
		void KnockBackAttack(const Math::Point2& pos);
	};
}


