#include "../ImGui/Docking/imgui_impl_win32.h"

#include "WindowsApp.h"

#define ID_BUTTON 100   //追加

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const float WindowsApp::window_width = 1280.0f;
const float WindowsApp::window_height = 768.0f;

const wchar_t WindowsApp::windowClassName[] = L"DirectXWindows";

//HINSTANCE hInst;
//HWND hBtn;//追加

// ウィンドウプロシージャ
LRESULT WindowsApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//int id;
	//char szBuf[64];

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}
	
	// メッセージで分岐
	switch (msg)
	{
	//case WM_CREATE:
	//	//ボタンの作成
	//	hBtn = CreateWindowEx(0, L"BUTTON", L"Push!!",
	//		WS_CHILD | WS_VISIBLE | BS_FLAT,
	//		10, 100, 50, 30,
	//		hwnd, (HMENU)ID_BUTTON, hInst, NULL);

	//	break;

	//case WM_COMMAND:
	//	switch (LOWORD(wparam)) {
	//	case ID_BUTTON:
	//		//ボタンが押されたときの処理
	//		MessageBox(hwnd, L"ボタンが押された！", L"Hey guys!", MB_OK);
	//		break;
	//	}
	//	break;

	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		//return 0;
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
	}
	return 0;
}

void WindowsApp::GenerateWindow()
{
	HICON hIcon = nullptr;
	if(hIcon == nullptr)hIcon = ExtractIcon((HINSTANCE)(GetModuleHandle(nullptr)),L"Resources/gameIcon.ico", 0);

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;// ウィンドウプロシージャを設定
	w.lpszClassName = windowClassName; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);// カーソル指定
	w.hIcon = hIcon;

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, (LONG)window_width, (LONG)window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,	// クラス名
			L"盤長",						// タイトルバーの文字
			WS_OVERLAPPEDWINDOW,			// タイトルバーと境界線があるウィンドウ
			CW_USEDEFAULT,					// 表示X座標（OSに任せる）
			CW_USEDEFAULT,					// 表示Y座標（OSに任せる）
			wrc.right - wrc.left,			// ウィンドウ横幅
			wrc.bottom - wrc.top,			// ウィンドウ縦幅
			nullptr,						// 親ウィンドウハンドル
			nullptr,						// メニューハンドル
			w.hInstance,					// 呼び出しアプリケーションハンドル
			nullptr);						// オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowsApp::Message()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		if (msg.message == WM_QUIT) {// 終了メッセージが来たらループを抜ける
			return true;
		}
		else {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
		}
	}

	return false;
}

void WindowsApp::TerminateWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}
