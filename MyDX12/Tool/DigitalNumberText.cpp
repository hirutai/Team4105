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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void DigitalNumberText::Print(int score, float x, float y, float scale)
{
	int posIndex = 0;
	std::string text = std::to_string(score);

	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない,かつ文字は'0'をはじめとする)
		const unsigned char& character = text[i];

		int fontIndex = character - 48;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// 文字を１つ進める
		spriteIndex++;
	}
}

void DigitalNumberText::SetColor(float r, float g, float b, float a)
{
	cr = r;
	cg = g;
	cb = b;
}

// まとめて描画
void DigitalNumberText::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->SetColor(cr, cg, cb, 1);
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}