#include "IntervalTimer.h" 
#include "../2D/Sprite.h"

using namespace XIIlib;

void IntervalTimer::Initialize(const int& timerTexNum, const int& barTexNum)
{
	timer = Sprite::Create(timerTexNum, { 10.0f, 100.0f }); // �^�C�}�[�摜�̍쐬
	timer_bar = Sprite::Create(barTexNum, { 20.0f, 150.0f }); // �^�C�}�[�o�[�摜�̍쐬
}

void IntervalTimer::Timer()
{
	if (currentSize <= 0) // �T�C�Y���O�ɂȂ�����
	{
		timerNum = 0; // �^�C�}�[�����Z�b�g
		currentSize = maxSize; // �T�C�Y�����Z�b�g
	}

	timerNum++; // �^�C�}�[

	currentSize = maxSize - intCountingNum * timerNum; // ���݂̃T�C�Y���v�Z

	timer_bar->SetSize({ currentSize, 50.0f });
}

void IntervalTimer::Draw()
{
	timer_bar->Draw();
	timer->Draw();
}