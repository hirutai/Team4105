#pragma once
//DigitalNumberText
#include <iostream>
class Sprite;

class DigitalNumberText
{
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 100;	// �ő啶����
	static const int fontWidth = 32;	// �t�H���g�摜��1�������̉���
	static const int fontHeight = 64;	// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 10;	// �t�H���g�摜��1�s���̕�����
	const int fontCode = -127;

public:// �ÓI�����o�֐�
	static DigitalNumberText* GetInstance();

public:// �����o�֐�

	void Initialize(unsigned int texnumber);

	void Print(int score, float x, float y, float scale = 1.0f);

	void SetColor(float r, float g, float b, float a);

	void DrawAll();

private:

	DigitalNumberText();
	DigitalNumberText(const DigitalNumberText&) = delete;
	~DigitalNumberText();
	DigitalNumberText& operator=(const DigitalNumberText&) = delete;

private:
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
	float cr = 1.0f;
	float cg = 1.0f;
	float cb = 1.0f;
};