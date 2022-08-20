#include "HPBar.h"

//#include "../3D/BillObj.h"
#include "../2D/Sprite.h"
#include "../2D/SpriteLoader.h"

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

	//timerBar = BillObj::Create({}, "timer_bar.png"); // �o�[�̃I�u�W�F�N�g�̍쐬
	//timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	//timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̐ݒ�
	//timerEdge = BillObj::Create({}, "timer_edge.png"); // ���̃I�u�W�F�N�g�̍쐬
	//timerEdge->SetSize(edgeSize); // ���̃T�C�Y�̐ݒ�
	//timerBar->SetPosition(hpBarPos.x - barSize.x / 2, hpBarPos.y, hpBarPos.z); // �o�[�̍��W�̐ݒ�(-�̓Y���̏C��)
	//timerEdge->SetPosition(hpBarPos.x, hpBarPos.y, hpBarPos.z); // ���̍��W�̐ݒ�
	timerBar = Sprite::Create(TIMER_BAR, {}); // �o�[�̃I�u�W�F�N�g�̍쐬
	//timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̐ݒ�
	timerEdge = Sprite::Create(TIMER_EDGE, {}); // �D�F�̃I�u�W�F�N�g�̍쐬
	timerEdge->SetSize(edgeSize); // ���̃T�C�Y�̐ݒ�
	timerBar->SetPosition({ hpBarPos.x + 10.0f, hpBarPos.y + 10.0f }); // �o�[�̍��W�̐ݒ�(-�̓Y���̏C��)
	timerEdge->SetPosition({ hpBarPos.x, hpBarPos.y}); // ���̍��W�̐ݒ�
}

void XIIlib::HPBar::Timer(int HP)
{
	decNum = HP * 40; // ���炷�ʂ̌v�Z

	currentSize = maxSize - (maxSize - HP * 40); // ���݂̃T�C�Y���v�Z

	barSize.x = currentSize; // �o�[�̃T�C�Y�̐ݒ�

	if (barSize.x < 0) {
		barSize.x = 0;
	}

	timerBar->SetSize(barSize); // �o�[�̃T�C�Y�̔��f
}

void HPBar::Draw()
{
	timerEdge->Draw(); // ���̍��W�̕`��
	timerBar->Draw(); // �o�[�̍��W�̕`��
}

void HPBar::SetPosition(const Vector3& position)
{
	timerBar->SetPosition({ position.x - 1.5f, position.y + 4 }); // �o�[�̍��W�̐ݒ�
	timerEdge->SetPosition({ position.x, position.y + 4 }); // ���̍��W�̐ݒ�
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