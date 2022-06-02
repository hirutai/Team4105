#pragma once
#include <iostream>
#include <string>
#include <DirectXMath.h>
class Sprite;
namespace Math
{
	struct Vector2;
	struct Vector3;
}
/// <summary>
/// デバッグ用文字表示
/// </summary>
class DebugText
{
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 512;	// 最大文字数
	static const int fontWidth = 18;	// フォント画像内1文字分の横幅
	static const int fontHeight = 36;	// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;	// フォント画像内1行分の文字数

public:// 静的メンバ関数
	static DebugText* GetInstance();

public:// メンバ関数

	void Initialize(unsigned int texnumber);

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void PrintVector2(Math::Vector2 vec, float x, float y, float scale);

	void PrintVector3(Math::Vector3 vec, float x, float y, float scale);

	void SetColor(float r, float g, float b, float a);

	void DrawAll();

private:
	DebugText();
	DebugText(const DebugText&) = delete;
	~DebugText();
	DebugText& operator=(const DebugText&) = delete;

private:
	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	DirectX::XMFLOAT4 colors;
};