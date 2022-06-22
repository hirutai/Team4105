#include "Curtain.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::Curtain* XIIlib::Curtain::Create(unsigned int textureNum, float x, float y)
{
	Curtain* pCurtain = new Curtain(textureNum,x,y);
	if (pCurtain == nullptr) {
		return nullptr;
	}

	pCurtain->Initialize();

	return pCurtain;
}

XIIlib::Curtain::Curtain(unsigned int textureNum, float x, float y)
{
	tex = Sprite::Create(textureNum, { x,y }, { 1,1,1,1 }, { 0.5f,0.5f });
	tex->SetSize({0.0f, 0.0f});
}

XIIlib::Curtain::~Curtain()
{
	delete tex;
}

void XIIlib::Curtain::Initialize()
{
	ResetTime();
	state = OPEN;
}

void XIIlib::Curtain::Draw()
{
	// �e�N�X�`���̕`��
	tex->Draw();
}

void XIIlib::Curtain::AddFrame()
{
	// �^�C�}�[�̐i�s
	time += (1.0f / 60.0f);
}

void XIIlib::Curtain::ResetTime()
{
	// ��Ƀ��Z�b�g
	time = 0.0f;
}

void XIIlib::Curtain::Open()
{
	// ACTIVE�Ŗ�����Δ�΂�
	if (GetState() != ACTIVE)return;
	// �T�C�Y�̒l���v�Z
	float newSize = (float)Easing::EaseInBack((double)GetTime(), 0, (double)texSize, (double)maxTime);
	// ���Ԃ�i�߂�
	AddFrame();
	// ���ʂ��擾
	float result = (float)texSize - newSize;
	// �T�C�Y���X�V
	SetSize(result, result);

	// ���Ԃ������Ȃ�����͔�΂�
	if (GetTime() < maxTime)return;
	ResetTime();
	SetState(OPEN);
	SetAlpha(0.0f);
}

void XIIlib::Curtain::Close()
{
	// ACTIVE�Ŗ�����Δ�΂�
	if (GetState() != ACTIVE)return;
	// �T�C�Y�̒l���v�Z
	float newSize = (float)Easing::EaseOutBack((double)GetTime(), 0, (double)texSize, (double)maxTime);
	// ���Ԃ�i�߂�
	AddFrame();
	// �T�C�Y���X�V
	SetSize(newSize, newSize);
	// ���Ԃ������Ȃ�����͔�΂�
	if (GetTime() < maxTime)return;
	ResetTime();
	SetState(CLOSE);
}

void XIIlib::Curtain::SetState(const TexState& cState)
{
	// ��Ԃ̍X�V
	state = cState;
}

void XIIlib::Curtain::SetSize(float x, float y)
{
	// �e�N�X�`���T�C�Y�̍X�V
	tex->SetSize({x,y});
}

void XIIlib::Curtain::SetAlpha(float a)
{
	tex->SetColor(1,1,1,a);
}
