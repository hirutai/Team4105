#pragma once
#include <Windows.h>

class WindowsApp
{
public: // ���ʍ���

	// �ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const float window_width;	// ����
	static const float window_height;	// �c��

	// �E�B���h�E�N���X�����������A�E�B���h�E�o�[�̖��O�ł͂Ȃ�
	static const wchar_t windowClassName[];

	// �ÓI�����o�֐�
	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	// �����o�֐�
	/// <summary>
	/// �Q�[���E�B���h�E�̍쐬
	/// </summary>
	void GenerateWindow();

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	/// <returns>�I�����ǂ���</returns>
	bool Message();

	/// <summary>
	/// �Q�[���E�B���h�E�̔j��
	/// </summary>
	void TerminateWindow();

	HWND GetWinHandle() { return hwnd; }

	HINSTANCE GetInstance() { return w.hInstance; }

private:
	// �����o�ϐ�
	// Window�֘A
	HWND hwnd = nullptr;// �E�B���h�E�n���h��
	WNDCLASSEX w{};// �E�B���h�E�N���X
};

