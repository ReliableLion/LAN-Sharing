// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


extern "C" __declspec(dllexport) VOID save_exception_callback(EXCEPTION_CALLBACK callback);
extern "C" __declspec(dllexport) VOID save_discovery_callback(DISCOVERY_MANAGED_CALLBACK callback);
extern "C" __declspec(dllexport) VOID save_accept_callback(ACCEPT_CALLBACK callback);
extern "C" __declspec(dllexport) VOID save_progress_bar_callback(PROGRESS_BAR_CALLBACK callback);

VOID save_exception_callback(EXCEPTION_CALLBACK callback) {
	managed_callback::getInstance().save_exit(callback);
}

VOID save_discovery_callback(DISCOVERY_MANAGED_CALLBACK callback) {
	managed_callback::getInstance().save_discovery(callback);
}

VOID save_accept_callback(ACCEPT_CALLBACK callback) {
	managed_callback::getInstance().save_accept(callback);
}

VOID save_progress_bar_callback(PROGRESS_BAR_CALLBACK callback) {
	managed_callback::getInstance().save_progress_bar_handler(callback);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		std::cout << "Lan-Sharing DLL correctly loaded" << std::endl;
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		std::cout << "Lan-Sharing DLL correctly unloaded" << std::endl;
		break;
    }
    return TRUE;
}