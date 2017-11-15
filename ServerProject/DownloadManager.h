/*#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <condition_variable>
#include <fstream>

#include "Buffer.h"
#include "RequestQueue.h"
#include "FileHandler.h"

#define THREAD_NUM_BIG 2
#define THREAD_NUM_SMALL 3 

// is the threshold value for the file 
#define FILE_THRESHOLD 4 * 1024 * 1024  // 4 MB 

class DownloadManager {
private:
	std::thread bigFileThread[THREAD_NUM_BIG];
	std::thread smallFileThread[THREAD_NUM_SMALL];

	// synchronization object for the first queue
	std::mutex BigFileMtx;
	std::mutex BigFileMtx_2;
	std::condition_variable BigFile_cv;

	// synchornization objcet for the second queue
	std::mutex SmallFileMtx;
	std::mutex SmallFileMtx_2;
	std::condition_variable SmallFile_cv;

	// termination flag
	std::atomic<bool> terminate;
	std::future<bool> file1;
	std::future<bool> file2;

	RequestQueue BigFileRequest_q;
	RequestQueue SmallFileRequest_q;

	std::string file_path = "";

	void _downloadFile(std::string filename, size_t size, std::shared_ptr<TCPconnection_server> conn);


public:
	DownloadManager() = default;
	~DownloadManager();
	void setupDownloader();
	void DownloadBigFile();
	void DownloadSmallFile();
	void InsertSmallFileRequest(size_t fileSize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection);
	void InsertBigFileRequest(size_t fileSize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection);
	void exitDownloader();
};

#endif // DONWLOAD_MANAGER_H*/