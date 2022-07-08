#include "HPBar.h"

#include "../3D/BillObj.h"

using namespace XIIlib;
using namespace Math;

HPBar* XIIlib::HPBar::GetInstance()
{
	static XIIlib::HPBar instance;
	return &instance;
}

HPBar::HPBar()
{
}

void HPBar::Initialize()
{
	currentSize = maxSize;

	timerBar = BillObj::Create({}, "timer_bar.png"); // �o�[�̃I�u�W�F�N�g�̍쐬
	timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̐ݒ�
	timerEdge = BillObj::Create({}, "timer_edge.png"); // ���̃I�u�W�F�N�g�̍쐬
	timerEdge->SetSize(edgeSize); // ���̃T�C�Y�̐ݒ�
	timerBar->SetPosition(hpBarPos.x - barSize.x / 2, hpBarPos.y, hpBarPos.z); // �o�[�̍��W�̐ݒ�(-�̓Y���̏C��)
	timerEdge->SetPosition(hpBarPos.x, hpBarPos.y, hpBarPos.z); // ���̍��W�̐ݒ�
}

void XIIlib::HPBar::Timer(float damage)
{
	decNum = damage; // ���炷�ʂ̌v�Z

	currentSize = currentSize - decNum; // ���݂̃T�C�Y���v�Z

	barSize.x = currentSize; // �o�[�̃T�C�Y�̐ݒ�

	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̔��f
}

void HPBar::Draw()
{
	timerBar->Draw(); // �o�[�̍��W�̕`��
	timerEdge->Draw(); // ���̍��W�̕`��
}

void HPBar::SetPosition(const Vector3& position)
{
	timerBar->SetPosition(position.x - 1.5f, position.y + 4, position.z - 1); // �o�[�̍��W�̐ݒ�
	timerEdge->SetPosition(position.x, position.y + 4, position.z - 1); // ���̍��W�̐ݒ�
}

bool XIIlib::HPBar::SizeZeroFlag()
{
	return currentSize <= 0;
}
bool XIIlib::HPBar::SizeThirdFlag()
{
	return currentSize == maxSize / 3;
}
bool XIIlib::HPBar::SizeThirdBelowFlag()
{
	return currentSize <= maxSize / 3;
}