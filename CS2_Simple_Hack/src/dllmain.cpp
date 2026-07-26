#include <Windows.h>
#include <thread>

#include "cs2/debug/debug.h"
#include "cs2/hook/hooks.h"

DWORD WINAPI Hack_Thread(LPVOID lpParam)
{   
	hook::Init(); // Initialize the hook

	while (!GetAsyncKeyState(VK_DELETE)) // Check if the END key is pressed
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100 milliseconds
	}

	hook::Shutdown(); // Shutdown the hook

	// Code to execute in the new thread
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, Hack_Thread, hModule, 0, nullptr); // Create a new thread to run the hack
		break;
	case DLL_THREAD_ATTACH:
		// Code to execute when a new thread is created in the process
		break;
	case DLL_THREAD_DETACH:
		// Code to execute when a thread exits cleanly
		break;
	case DLL_PROCESS_DETACH:
		// Code to execute when the DLL is unloaded from a process
		break;
	}
	return TRUE;
}