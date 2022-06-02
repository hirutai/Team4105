#pragma once
#include <Windows.h>

class WindowsApp
{
public: // 共通項目

	// 静的メンバ変数
	// ウィンドウサイズ
	static const float window_width;	// 横幅
	static const float window_height;	// 縦幅

	// ウィンドウクラス名※ただし、ウィンドウバーの名前ではない
	static const wchar_t windowClassName[];

	// 静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	// メンバ関数
	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	void GenerateWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool Message();

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void TerminateWindow();

	HWND GetWinHandle() { return hwnd; }

	HINSTANCE GetInstance() { return w.hInstance; }

private:
	// メンバ変数
	// Window関連
	HWND hwnd = nullptr;// ウィンドウハンドル
	WNDCLASSEX w{};// ウィンドウクラス
};

