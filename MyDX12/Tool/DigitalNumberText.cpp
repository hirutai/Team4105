#include "DigitalNumberText.h"
#include "../2D/Sprite.h"
#include <string>
#include <DirectXMath.h>

DigitalNumberText::DigitalNumberText()
{
	//colors = { 1,1,1,1 };
}

DigitalNumberText::~DigitalNumberText()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

DigitalNumberText* DigitalNumberText::GetInstance()
{
	static DigitalNumberText instance;
	return &instance;
}

void DigitalNumberText::Initialize(unsigned int texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void DigitalNumberText::Print(int score, float x, float y, float scale)
{
	int posIndex = 0;
	std::string text = std::to_string(score);

	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�,��������'0'���͂��߂Ƃ���)
		const unsigned char& character = text[i];

		int fontIndex = character - 48;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// �������P�i�߂�
		spriteIndex++;
	}
}

void DigitalNumberText::SetColor(float r, float g, float b, float a)
{
	cr = r;
	cg = g;
	cb = b;
}

// �܂Ƃ߂ĕ`��
void DigitalNumberText::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->SetColor(cr, cg, cb, 1);
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}