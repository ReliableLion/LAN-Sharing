#pragma once
#include "pch.h"
#include <string>

class managed_callback {

public:
	static managed_callback& getInstance() {
		static managed_callback instance;
		return instance;
	}
	managed_callback(managed_callback const&) = delete;
	void operator=(managed_callback const&) = delete;

	void save_exit(EXCEPTION_CALLBACK callback) {
		exit_callback = callback;
	}

	void save_discovery(DISCOVERY_MANAGED_CALLBACK callback) {
		discovery_managed_callback = callback;
	}

	void call_exit() {
		exit_callback();
	}

	void call_discovery(std::string address, std::string username, std::string avatar) {
		discovery_managed_callback(address.c_str(), username.c_str(), avatar.c_str());
	}

private:
	managed_callback() = default;
	EXCEPTION_CALLBACK exit_callback;
	DISCOVERY_MANAGED_CALLBACK discovery_managed_callback;
};
