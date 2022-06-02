#pragma once
#include <iostream>
#include <string>
#include <DirectXMath.h>
class Sprite;

class DebugJISText
{
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 512;	// �ő啶����
	static const int fontWidth = 32;	// �t�H���g�摜��1�������̉���
	static const int fontHeight = 32;	// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 189;	// �t�H���g�摜��1�s���̕�����
	const int fontCode = -127;

public:// �ÓI�����o�֐�
	static DebugJISText* GetInstance();

public:// �����o�֐�

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
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	DirectX::XMFLOAT4 colors;
};