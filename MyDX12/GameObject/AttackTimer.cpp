#include "AttackTimer.h"

#include "../2D/Sprite.h"

using namespace XIIlib;

void AttackTimer::Initialize(const int& timerTexNum, const int& barTexNum)
{
	timer_bar = Sprite::Create(barTexNum, { 20.0f, 150.0f }); // �^�C�}�[�o�[�摜�̍쐬
}

void AttackTimer::Timer()
{
	if (currentSize <= 0) // �T�C�Y���O�ɂȂ�����
	{
		timerNum = 0; // �^�C�}�[�����Z�b�g
		currentSize = maxSize; // �T�C�Y�����Z�b�g
	}

	timerNum++; // �^�C�}�[

	currentSize = maxSize - decNum * timerNum; // ���݂̃T�C�Y���v�Z

	timer_bar->SetSize({ currentSize, 50.0f }); // �T�C�Y�̐ݒ�
}

void AttackTimer::Draw()
{
	timer_bar->Draw();
}