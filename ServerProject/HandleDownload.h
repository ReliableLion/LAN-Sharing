#ifndef HANDLE_DOWNLOAD
#define HANDLE_DOWNLOAD

#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <condition_variable>
#include <fstream>
#include "RequestQueue.h"

#define THREAD_NUM_BIG 2
#define THREAD_NUM_SMALL 3 

// is the threshold value for the file 
#define FILE_THRESHOLD 4 * 1024 * 1024  // 4 MB 


class HandleDownload {
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

	RequestQueue BigFileRequest_q;
	RequestQueue SmallFileRequest_q;

public:
	HandleDownload();
	~HandleDownload();
	void DownloadBigFile();
	void DownloadSmallFile();
	void InsertSmallFileRequest(size_t fileSize, std::string fileName, std::shared_ptr<TCPconnection> new_connection);
	void InsertBigFileRequest(size_t fileSize, std::string fileName, std::shared_ptr<TCPconnection> new_connection);
};

#endif // HANDLE_DOWNLOAD