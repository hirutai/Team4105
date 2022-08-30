#pragma once
#include "Boss.h"
#include "../Struct/Math.h"
#include <string>

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
		static const int SHOW_TILE_SPACE = 7; // 7�t���[���Ԋu�ŕ\��
		static const int HIDE_TILE_SPACE = 7; // 7�t���[���Ԋu�Ŕ�\��
		static const int MAX_TILE = 8; // �^�C���̍ő吔
		static const int METEORS_MAX = 15; // ���e�I�U���̃��e�I��
		const Math::Point2 METEOR_MIN_MAX = {0,7};
		const Math::Point2 BOSSTILE_MIN_MAX = { 0,5 };

	private: // �����o�ϐ�
		int positionType = 0; // PositionType��int�^�ŊǗ�
		int frameCount = 0; // �J�E���g
		int numbersA[METEORS_MAX] = {}; // �����_�����e�I�̍��W�i�[�z��A
		int numbersB[METEORS_MAX] = {}; // �����_�����e�I�̍��W�i�[�z��B
		int tileNum = 0; // �^�C���i���o�[
		bool tilesShowFlag[MAX_TILE] = {}; // �^�C���\���̐���
		bool tileSwitch = true; // �^�C����\�����邩��\���ɂ��邩
		int attackFrameCnt = 0; // �U���t���[���̃J�E���g
		int meteorsCount = -20; // ���e�Is�̃J�E���g
		int targetTileRand = 0; // Line�U�����̃����_�����W�i�[

	public: // �ÓI�֐�
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
		/// �^�C���̕\���J�ڗp
		/// </summary>
		void DispTileDeathControl(const int& bossAttackSelect);
		/// <summary>
		/// Line���߂̊j�����̍��W�������_������
		/// </summary>
		void Target();
		/// <summary>
		/// �����_���ȃ��e�I�𐶐�(�d���Ȃ�)
		/// </summary>
		void CreateMeteorPosition(int& bossAttackSelect);
		/// <summary>
		/// ���e�I�����̃��e�I�Əd�Ȃ��Ă��Ȃ����`�F�b�N
		/// </summary>
		bool CheckMeteorArea(const Math::Point2& meteorPos);

		/// <summary>
		/// PositionType����
		/// </summary>
		/// <param name="type"></param>
		void AssignPositionType(const std::string& type);
	public: // Getter Setter
		int GetTargetTile() { return targetTileRand; }

	public: // ------------------ �\���U���I���n ------------------
		/* Attack�ōU�� Display�ŕ\��*/
		/// <summary>
		/// �c����3��\��
		/// </summary>
		void Vertical3Line(const std::string& type);
		/// <summary>
		/// ������3��\��
		/// </summary>
		void Horizontal3Line(const std::string& type);
		/// <summary>
		/// 1�_ 3x3���e�I
		/// </summary>
		void OneMeteor3x3(const std::string& type, const Math::Point2& kingPos);
	public: // �\���U�������n
		/// <summary>
		/// �����_���܂΂� 1x1���e�I
		/// </summary>
		void RandomMeteor1x1Attack();
		// �ߋ����m�b�N�o�b�N�U��(����1�}�X)
		void KnockBackAttack(const Math::Point2& pos);
	};
}


