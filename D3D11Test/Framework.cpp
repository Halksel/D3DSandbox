#include "Framework.h"


	Framework Framework::Instance;

	Framework::Framework(void)
	{
	}

	Framework::~Framework(void)
	{
	}

	// 初期化
	bool Framework::Initialize(const FrameworkDesc& desc)
	{
		// Windowの生成
		if (!m_Window.Create(desc.Name, desc.X, desc.Y, desc.Width, desc.Height))
		{
			return false;
		}
		// DirectX11の初期化
		if (!m_DirectX11.Initialize(m_Window))
		{
			return false;
		}

		return true;
	}


	// 終了処理
	void Framework::Finalize(void)
	{
		m_DirectX11.Finalize();
	}

	void Framework::MainLoop() {
		while (!m_Window.IsQuitMessage())
		{
			if (!m_Window.UpdateMessage())
			{
				m_DirectX11.Render();
			}
		}

	}
