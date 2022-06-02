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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void DebugJISText::Print(const std::string & text, float x, float y, float scale)
{
	int posIndex = 0;

	// 全ての文字について
	for (int i = 0; i < text.size(); i+= 2)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
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

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * posIndex, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// 文字を１つ進める
		spriteIndex++;
		posIndex++;
	}
}

void DebugJISText::SetColor(float r, float g, float b, float a)
{
	colors = { r,g,b,a };
}

// まとめて描画
void DebugJISText::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->SetColor(colors.x, colors.y, colors.z, colors.w);
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}