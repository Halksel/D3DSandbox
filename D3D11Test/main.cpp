#include "Framework.h"



// エントリーポイント
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	FrameworkDesc desc;
	desc.Name = TEXT("DirectX11");
	desc.X = 0;
	desc.Y = 0;
	desc.Width = 1280;
	desc.Height = 720;

	Framework& framework = Framework::GetInstance();
	if (!framework.Initialize(desc))
	{
		return false;
	}

	framework.MainLoop();

	framework.Finalize();

	return 0;
}
