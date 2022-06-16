#include "AttackTimer.h"

#include "../3D/BillObj.h"

using namespace XIIlib;
using namespace Math;

AttackTimer::AttackTimer(float countingNum_)
{
	countingNum = countingNum_; // �J�E���g���������̓���
	decNum = baseDecNum / countingNum; //  // ���炷�ʂ̌v�Z
	timerNum = 0;
	currentSize = maxSize;
}

void AttackTimer::Initialize()
{
	timerBar = BillObj::Create({}, "timer_bar.png"); // �o�[�̃I�u�W�F�N�g�̍쐬
	timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̐ݒ�
	timerEdge = BillObj::Create({}, "timer_edge.png"); // ���̃I�u�W�F�N�g�̍쐬
	timerEdge->SetSize(edgeSize); // ���̃T�C�Y�̐ݒ�
}

void AttackTimer::Timer()
{
	if (SizeZeroFlag()) // �T�C�Y���O�ɂȂ�����
	{
		timerNum = 0; // �^�C�}�[�����Z�b�g
		currentSize = maxSize; // �T�C�Y�����Z�b�g
	}

	timerNum++; // �^�C�}�[�J�E���g��

	currentSize = maxSize - decNum * timerNum; // ���݂̃T�C�Y���v�Z

	barSize.x = currentSize; // �o�[�̃T�C�Y�̐ݒ�

	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̔��f
}

void AttackTimer::Draw()
{
	timerBar->Draw(); // �o�[�̍��W�̕`��
	timerEdge->Draw(); // ���̍��W�̕`��
}

void AttackTimer::SetPosition(const Vector3& position)
{
	timerBar->SetPosition(position.x - 1.5f, position.y + 4, position.z - 1); // �o�[�̍��W�̐ݒ�
	timerEdge->SetPosition(position.x, position.y + 4, position.z - 1); // ���̍��W�̐ݒ�
}

bool XIIlib::AttackTimer::SizeZeroFlag()
{
	return currentSize <= 0;
}
