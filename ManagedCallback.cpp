#include "pch.h"

using namespace managed_callback;

extern "C" __declspec(dllexport) VOID save_exception_callback(EXCEPTION_CALLBACK callback);
extern "C" __declspec(dllexport) VOID save_discovery_callback(DISCOVERY_MANAGED_CALLBACK callback);

VOID save_exception_callback(EXCEPTION_CALLBACK callback) {
	exit_callback = callback;
}

VOID save_discovery_callback(DISCOVERY_MANAGED_CALLBACK callback) {
	discovery_managed_callback = callback;
}
