#pragma once

#include <windows.h>
#include <stdio.h>

// SimpleWindowクラス
class SimpleWindow
{
public:
	SimpleWindow(void);
	~SimpleWindow(void);


	// シンプルなウィンドウの作成
	bool Create(LPCTSTR pName, int x, int y, int width, int height);
	// ウィンドウを閉じる
	void Quit(void);
	// メッセージの更新
	bool UpdateMessage(void);

public:
	// 終了通知が来ているか？
	bool IsQuitMessage(void) { return m_isQuitMessage; }
	// ウィンドウハンドルの取得
	HWND GetHandle(void) { return m_hWnd; }

	// ウィンドウの横幅取得
	int GetWidth(void) const { return m_width; }
	// ウィンドウの縦幅取得
	int GetHeight(void) const { return m_height; }

private:
	// ウィンドウハンドル
	HWND	m_hWnd;
	// 終了通知が来ているか？
	bool	m_isQuitMessage;

	// ウィンドウサイズ
	int		m_width;
	int		m_height;
};
