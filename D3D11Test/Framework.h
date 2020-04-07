#pragma once

#include "Window.h"
#include "DirectX11.h"
#include "Shader.h"

struct FrameworkDesc
{
	LPCTSTR Name;
	int X, Y;
	int Width, Height;
};

// Frameworkクラス
class Framework
{
public:
	Framework(void);
	~Framework(void);

	// 初期化
	bool Initialize(const FrameworkDesc& desc);
	// 終了処理
	void Finalize(void);

	// ウィンドウの取得
	SimpleWindow& GetWindow(void) { return m_Window; }
	// DirectX11の取得
	DirectX11& GetDirectX11(void) { return m_DirectX11; }

	void MainLoop();

	static Framework& GetInstance(void) { return Instance; }
private:
	SimpleWindow m_Window;	// ウィンドウ
	DirectX11 m_DirectX11;	// DirectX11

	// シェーダー
	VertexShader m_vs;
	PixelShader m_ps;

	static Framework Instance;	// インスタンス
};
