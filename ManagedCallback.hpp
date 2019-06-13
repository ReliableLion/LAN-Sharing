#pragma once
#include "pch.h"

namespace managed_callback {
	static EXCEPTION_CALLBACK exit_callback;
	static DISCOVERY_MANAGED_CALLBACK discovery_managed_callback;

	static VOID save_exception_callback(EXCEPTION_CALLBACK callback);
	static VOID save_discovery_callback(DISCOVERY_MANAGED_CALLBACK callback);
}
