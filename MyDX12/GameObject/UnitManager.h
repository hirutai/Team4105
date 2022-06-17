#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Unit.h"

namespace XIIlib {

	class BoardMap;
	class IntervalTimer;

	class UnitManager {
	private:
		UnitManager();
		~UnitManager();

		std::vector<std::shared_ptr<Unit>> units;
		std::vector<std::shared_ptr<Unit>> add_units;

		IntervalTimer* _intervalTimter = nullptr;// �R�s�[�p

		BoardMap* map_board = nullptr;

		int deadCount = 0;

	public:
		UnitManager(const UnitManager& obj) = delete;
		UnitManager& operator = (const UnitManager& obj) = delete;

		static UnitManager* GetInstance();

		void Initialize(); // ������

		void Update(); // �X�V

		void Draw(); // �`��

		void AddUnit(const std::shared_ptr<Unit>& spObject); // ���j�b�g�̒ǉ�

		void DeadUnit();

		// �R���e�i���炷�ׂč폜
		void AllDestroy();

		// �w��}�X�Ƀ��j�b�g���ڂ��Ă��邩?
		bool AllOnUnit(const Math::Point2& point);

		// �w�蕶���񂩂烆�j�b�g��"ID(�N���X��)"���q�b�g�����v�f����Ԃ�
		int GetUnitIDElements(std::string name)const;
		// ���j�b�g���擾
		std::shared_ptr<Unit> GetUnit(int index)const;
		// �����̗v�f�����擾
		std::vector<int> GetUnitNumbers(std::string name)const;

		void SetBackVector(const Math::Point2& element, const Math::Point2& point);

		// �w��}�X�̍U�������L����
		void ChangeAttackValidTile(const Math::Point2& element, int type_positioning);

		void SetOnTile(const Math::Point2& element);

		bool IsAttackValid(const Math::Point2& element, int type_positioning)const;

		Math::Point2 GetBackVector(const Math::Point2& element)const;

		bool GetOnTile(const Math::Point2& element)const;

		std::vector<int> GetOnUnitElements(const Math::Point2& point);

		// �^�C�}�[��GetSet�֐�
		/// <summary>
		/// IntervalTimer�^��ݒ�
		/// </summary>
		/// <param name="intervalTimer">�C���^�[�o���^�C��</param>
		void SetIntervalTimer(IntervalTimer* intervalTimer) { _intervalTimter = intervalTimer; }

		/// <summary>
		/// Timer�̎擾
		/// </summary>
		/// <returns> unsigned int �^�C�}�[��Ԃ�</returns>
		const unsigned int GetIntervalTimer();

		// �G�����񂾎��ɃJ�E���g�֐�
		void AddCountDead();
		// �J�E���g�̃��Z�b�g
		void ResetCount();

	};

}