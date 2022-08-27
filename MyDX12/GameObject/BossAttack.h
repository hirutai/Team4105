#pragma once
#include "Boss.h"
#include "../Struct/Math.h"

// ����̗\��A�U���ƕ\���̊֐����킹�邩��

namespace XIIlib {
	// �{�X�A�^�b�N�N���X
	class BossAttack
	{
	private: // �V���O���g����
		BossAttack() = default;
		~BossAttack() = default;
		BossAttack(const BossAttack&) = delete;
		BossAttack& operator = (const BossAttack&) = delete;
	private: // �萔
		static const int DISPLAY_FRAME = 12; // 12�t���[���Ԋu�ŕ\��
		static const int MAX_TILE = 8; // �^�C���̍ő吔
		static const int METEORS_MAX = 15; // ���e�I�U���̃��e�I��

	private: // �����o�ϐ�
		int count = 0;
		int numbersA[METEORS_MAX]; // �����_�����e�I�̍��W�i�[�z��A
		int numbersB[METEORS_MAX]; // �����_�����e�I�̍��W�i�[�z��B
		int tileNum = 0; // �^�C���i���o�[
		bool tileDeth[MAX_TILE] = {}; // �^�C���\���̐���
		int attackFrameCnt = 0; // �U���t���[���̃J�E���g
		int meteorsCount = -20; // ���e�Is�̃J�E���g

		int bossMin = 0;
		int bossMax = 5;
		int bossTileRand = 0;

		int meteorAMin = 0;
		int meteorAMax = 7;
		int meteorBMin = 0;
		int meteorBMax = 7;

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
		void DispTileDeathControl(const int& bossAttackSelect);
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
		// void RandomMeteor1x1Display();

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

	public: // �\���U������
		/// <summary>
		/// �����_���܂΂� 1x1���e�I
		/// </summary>
		void RandomMeteor1x1Attack();
		// �ߋ����m�b�N�o�b�N�U��(����1�}�X)
		void KnockBackAttack(const Math::Point2& pos);
	};
}


