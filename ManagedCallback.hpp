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

	void save_accept(ACCEPT_CALLBACK accept_callback) {
		this->accept_callback = accept_callback;
	}

	void save_progress_bar_handler(PROGRESS_BAR_CALLBACK progress_bar_callback) {
		this->progress_bar_callback = progress_bar_callback;
	}

	void save_file_sent_handler(FILE_COMPLETE_CALLBACK file_sent_callback) {
		this->file_sent_callback = file_sent_callback;
	}

	void save_file_downloaded_callback(FILE_COMPLETE_CALLBACK callback) {
		this->file_downloaded_callback = callback;
 	}

	void save_begin_download_callback(BEGIN_DOWNLOAD_CALLBACK begin_download_callback) {
		this->begin_download_callback = begin_download_callback;
	}

	void call_exit() {
		exit_callback();
	}

	void call_discovery(std::string address, std::string username, std::string avatar) {
		discovery_managed_callback(address.c_str(), username.c_str(), avatar.c_str());
	}

	bool call_accept_callback(std::string filename) {
		return accept_callback(filename.c_str());
	}

	void call_progress_bar_callback(const char *id, int progress) {
		progress_bar_callback(id, progress);
	}

	void call_file_sent_callback(const char *id, bool finished) {
		file_sent_callback(id, finished);
	}

	void call_file_download_callback(const char *id, bool finished) {
		file_downloaded_callback(id, finished);
	}

	void call_begin_download_callback(const char *id, const char *file_path) {
		begin_download_callback(id, file_path);
	}

private:
	managed_callback() = default;
	EXCEPTION_CALLBACK exit_callback;
	DISCOVERY_MANAGED_CALLBACK discovery_managed_callback;
	ACCEPT_CALLBACK accept_callback;
	PROGRESS_BAR_CALLBACK progress_bar_callback;
	FILE_COMPLETE_CALLBACK file_sent_callback;
	FILE_COMPLETE_CALLBACK file_downloaded_callback;
	BEGIN_DOWNLOAD_CALLBACK begin_download_callback;
};
