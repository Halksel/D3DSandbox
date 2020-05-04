////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;
	// Create the system object.
	SystemClass& system = Singleton<SystemClass>::get_instance();

	// Initialize and run the system object.
	result = system.Initialize();
	if(result)
	{
		system.Run();
	}

	// Shutdown and release the system object.
	system.Shutdown();

	SingletonFinalizer::finalize();

	return 0;
}