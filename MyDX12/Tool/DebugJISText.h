#pragma once
#include <iostream>
#include <string>
#include <DirectXMath.h>
class Sprite;

class DebugJISText
{
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 512;	// 最大文字数
	static const int fontWidth = 32;	// フォント画像内1文字分の横幅
	static const int fontHeight = 32;	// フォント画像内1文字分の縦幅
	static const int fontLineCount = 189;	// フォント画像内1行分の文字数
	const int fontCode = -127;

public:// 静的メンバ関数
	static DebugJISText* GetInstance();

public:// メンバ関数

	void Initialize(unsigned int texnumber);

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void SetColor(float r, float g, float b, float a);

	void DrawAll();

private:

	DebugJISText();
	DebugJISText(const DebugJISText&) = delete;
	~DebugJISText();
	DebugJISText& operator=(const DebugJISText&) = delete;

private:
	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	DirectX::XMFLOAT4 colors;
};