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
/// �f�o�b�O�p�����\��
/// </summary>
class DebugText
{
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 512;	// �ő啶����
	static const int fontWidth = 18;	// �t�H���g�摜��1�������̉���
	static const int fontHeight = 36;	// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;	// �t�H���g�摜��1�s���̕�����

public:// �ÓI�����o�֐�
	static DebugText* GetInstance();

public:// �����o�֐�

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
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	DirectX::XMFLOAT4 colors;
};