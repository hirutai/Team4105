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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void DebugText::Print(const std::string & text, float x, float y, float scale)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
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

// まとめて描画
void DebugText::DrawAll()
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