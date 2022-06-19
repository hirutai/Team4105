#include "Unit.h"
#include "UnitManager.h"
#include "../GameObject/Common.h"
#include <iostream>
#include "../3D/Object3D.h"

bool XIIlib::Unit::ThreeCheckArea(Math::Point2 element_stock)
{
	if (UnitManager::GetInstance()->AllOnUnit(element_stock)
		|| Common::GetExceptionPoint(element_stock.a)
		|| Common::GetExceptionPoint(element_stock.b)) return true;

	return false;

}

void XIIlib::Unit::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Unit::BillObjectDraw()
{
}

void XIIlib::Unit::ZeroHp()
{
	if (_hit_point <= 0)
	{
		OnDead();
		UnitManager::GetInstance()->AddCountDead();
	}
}

void XIIlib::Unit::Draw()
{
	// �I�u�W�F�N�g�̕`��
	object3d->Draw();
}

void XIIlib::Unit::KnockBack()
{
	// �U���������Ă��Ȃ���΂����Ԃ�
	if (!UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))return;

	SetHitDamage(1);
	isAttack = false;
	// �m�b�N�o�b�N�̈ړ���
	const Math::Point2 backVector = UnitManager::GetInstance()->GetBackVector(element_stock);

	// �m�b�N�o�b�N
	const int countLoop = 2;
	for (int i = 0; i < countLoop; i++) {
		Math::Point2 movePoint = backVector / 2.0f;
		if (Common::GetExceptionPoint(element_stock.a + movePoint.a) || Common::GetExceptionPoint(element_stock.b + movePoint.b)) {
			movePoint *= 0;
		}
		element_stock += movePoint;
	}
	Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp = kingPos;
	// �m�b�N�o�b�N���ɊԂɑ��̋����ꍇ�A���̊Ԃ̋�̈ʒu�Ɏ~�܂�
	// �c�Ƀm�b�N�o�b�N
	if (dif.a == 0)
	{
		// 0��菬�������King����ɂ���
		if (dif.b < 0)
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.b) - 1; ++i)
			{
				temp.b++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
				}
			}
		}
		else // 0���傫�����King��艺�ɂ���
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.b) - 1; ++i)
			{
				temp.b--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
				}
			}
		}
	}
	else if (dif.b == 0) // ���Ƀm�b�N�o�b�N
	{
		// 0��菬�������King���E
		if (dif.a < 0)
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
				}
			}
		}
		else// 0���傫�����King��荶
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
				}
			}
		}
	}
	else	// �΂߂Ƀm�b�N�o�b�N
	{
		// ����Ƀm�b�N�o�b�N
		if (dif.a > 0 && dif.b < 0)
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a--;
				temp.b++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;

				}
			}
		}
		else if (dif.a < 0 && dif.b < 0)	// �E��Ƀm�b�N�o�b�N
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a++;
				temp.b++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
					break;
				}
			}
		}
		else if (dif.a > 0 && dif.b > 0)	// �����Ƀm�b�N�o�b�N
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a--;
				temp.b--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
					break;
				}
			}
		}
		else if (dif.a < 0 && dif.b > 0)	// �E���Ƀm�b�N�o�b�N
		{
			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
			for (int i = 0; i < abs(dif.a) - 1; ++i)
			{
				temp.a++;
				temp.b--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))
				{
					element_stock = temp;
					break;
				}
			}
		}
	}
}

void XIIlib::Unit::SetElementStock(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::Unit::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
}
