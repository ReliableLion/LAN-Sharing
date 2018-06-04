#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "ConcurrentQueue.hpp"
#include "FileHandler.hpp"
#include "Connection.hpp"
#include "Message.hpp"

typedef struct {
    request_struct req;
    connection::conn_ptr conn;
} download_struct;

class DownloadManager {
    const int MAX_THREAD_B_ = BIG_FILE_THREADS;
    const int MAX_THREAD_S_ = SMALL_FILE_THREADS;
    std::vector<std::thread> thread_pool_b_;
    std::vector<std::thread> thread_pool_s_;
    ConcurrentQueue<download_struct> big_file_q_;
    ConcurrentQueue<download_struct> small_file_q_;
    std::mutex mtx_b_;
    std::mutex mtx_s_;
    std::atomic<bool> is_terminated_;
    std::condition_variable cv_b_, cv_s_;

    // file variables
    std::string path_;
    const std::string TEMP_PATH_ = TEMP_PATH;

    void process_big_file();

    void process_small_file();

	static bool download_file(download_struct request, TemporaryFile &temporary_file);

	static bool copy_file(TemporaryFile &temporary_file, FileHandler &destination_file);

public:
    DownloadManager();

    ~DownloadManager();

    void terminate_service();

    bool insert_small_file(request_struct request, connection::conn_ptr connection);

    bool insert_big_file(request_struct request, connection::conn_ptr connection);
};