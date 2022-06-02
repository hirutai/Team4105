#include "DebugText.h"
#include "../2D/Sprite.h"
#include <string>

DebugText::DebugText()
{
	colors = { 1,1,1,1 };
}

DebugText::~DebugText()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

DebugText * DebugText::GetInstance()
{
	static DebugText instance;
	return &instance;
}

void DebugText::Initialize(unsigned int texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void DebugText::Print(const std::string & text, float x, float y, float scale)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
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

void DebugText::PrintVector2(Math::Vector2 vec, float x, float y, float scale)
{
	std::string a = "(" + std::to_string(vec.x);
	std::string b = std::to_string(vec.y) + ")";
	Print(a + "," +  b , x, y, scale);
}

void DebugText::PrintVector3(Math::Vector3 vec, float x, float y, float scale)
{
	std::string a = "X : " + std::to_string(vec.x);
	std::string b = "Y : " + std::to_string(vec.y);
	std::string c = "Z : " + std::to_string(vec.z);
	Print(a, x, y + ((fontHeight * scale) * 0), scale);
	Print(b, x, y + ((fontHeight * scale) * 1), scale);
	Print(c, x, y + ((fontHeight * scale) * 2), scale);
}

void DebugText::SetColor(float r, float g, float b, float a)
{
	colors = { r,g,b,a };
}

// �܂Ƃ߂ĕ`��
void DebugText::DrawAll()
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