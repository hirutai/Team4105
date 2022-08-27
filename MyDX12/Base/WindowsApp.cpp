#include "../ImGui/Docking/imgui_impl_win32.h"

#include "WindowsApp.h"

#define ID_BUTTON 100   //�ǉ�

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const float WindowsApp::window_width = 1280.0f;
const float WindowsApp::window_height = 768.0f;

const wchar_t WindowsApp::windowClassName[] = L"DirectXWindows";

//HINSTANCE hInst;
//HWND hBtn;//�ǉ�

// �E�B���h�E�v���V�[�W��
LRESULT WindowsApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//int id;
	//char szBuf[64];

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}
	
	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	//case WM_CREATE:
	//	//�{�^���̍쐬
	//	hBtn = CreateWindowEx(0, L"BUTTON", L"Push!!",
	//		WS_CHILD | WS_VISIBLE | BS_FLAT,
	//		10, 100, 50, 30,
	//		hwnd, (HMENU)ID_BUTTON, hInst, NULL);

	//	break;

	//case WM_COMMAND:
	//	switch (LOWORD(wparam)) {
	//	case ID_BUTTON:
	//		//�{�^���������ꂽ�Ƃ��̏���
	//		MessageBox(hwnd, L"�{�^���������ꂽ�I", L"Hey guys!", MB_OK);
	//		break;
	//	}
	//	break;

	case WM_DESTROY: // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		//return 0;
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
	}
	return 0;
}

void WindowsApp::GenerateWindow()
{
	HICON hIcon = nullptr;
	if(hIcon == nullptr)hIcon = ExtractIcon((HINSTANCE)(GetModuleHandle(nullptr)),L"Resources/gameIcon.ico", 0);

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;// �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = windowClassName; // �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);// �J�[�\���w��
	w.hIcon = hIcon;

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, (LONG)window_width, (LONG)window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,	// �N���X��
			L"�Ւ�",						// �^�C�g���o�[�̕���
			WS_OVERLAPPEDWINDOW,			// �^�C�g���o�[�Ƌ��E��������E�B���h�E
			CW_USEDEFAULT,					// �\��X���W�iOS�ɔC����j
			CW_USEDEFAULT,					// �\��Y���W�iOS�ɔC����j
			wrc.right - wrc.left,			// �E�B���h�E����
			wrc.bottom - wrc.top,			// �E�B���h�E�c��
			nullptr,						// �e�E�B���h�E�n���h��
			nullptr,						// ���j���[�n���h��
			w.hInstance,					// �Ăяo���A�v���P�[�V�����n���h��
			nullptr);						// �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowsApp::Message()
{
	MSG msg{};	// ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		if (msg.message == WM_QUIT) {// �I�����b�Z�[�W�������烋�[�v�𔲂���
			return true;
		}
		else {
			TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}
	}

	return false;
}

void WindowsApp::TerminateWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}
