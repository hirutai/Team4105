#include "DebugJISText.h"
#include "../2D/Sprite.h"

DebugJISText::DebugJISText()
{
	colors = { 1,1,1,1 };
}

DebugJISText::~DebugJISText()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

DebugJISText* DebugJISText::GetInstance()
{
	static DebugJISText instance;
	return &instance;
}

void DebugJISText::Initialize(unsigned int texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void DebugJISText::Print(const std::string & text, float x, float y, float scale)
{
	int posIndex = 0;

	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i+= 2)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& first = text[i];
		const unsigned char& second = text[i + 1];

		int fontIndex = 0;

		for (int j = 0; j < 0x1e; j++)
		{
			if (text[i] == fontCode + j)
			{
				//0x7c
				fontIndex = second - 0x40 + (fontLineCount * j);
			}
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * posIndex, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// �������P�i�߂�
		spriteIndex++;
		posIndex++;
	}
}

void DebugJISText::SetColor(float r, float g, float b, float a)
{
	colors = { r,g,b,a };
}

// �܂Ƃ߂ĕ`��
void DebugJISText::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->SetColor(colors.x, colors.y, colors.z, colors.w);
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}