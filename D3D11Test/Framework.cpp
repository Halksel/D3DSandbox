#include "Framework.h"


	Framework Framework::Instance;

	Framework::Framework(void)
	{
	}

	Framework::~Framework(void)
	{
	}

	// ‰Šú‰»
	bool Framework::Initialize(const FrameworkDesc& desc)
	{
		// Window‚Ì¶¬
		if (!m_Window.Create(desc.Name, desc.X, desc.Y, desc.Width, desc.Height))
		{
			return false;
		}
		// DirectX11‚Ì‰Šú‰»
		if (!m_DirectX11.Initialize(m_Window))
		{
			return false;
		}

		return true;
	}


	// I—¹ˆ—
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
