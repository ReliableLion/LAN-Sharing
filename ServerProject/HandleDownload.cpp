#include "HandleDownload.h"

// when the costructor of this object is invoked the threads are createsd and go into sleep mode
void HandleDownload::setupDownloader() {

	terminate.store(false);

	for (int i = 0; i < THREAD_NUM_BIG; i++) {
		bigFileThread[i] = std::thread(&HandleDownload::DownloadBigFile, this);
	}

	for (int i = 0; i < THREAD_NUM_SMALL; i++) {
		smallFileThread[i] = std::thread(&HandleDownload::DownloadSmallFile, this);
	}
}


// define a method to exit 
HandleDownload::~HandleDownload() {

	terminate.store(true);

	for (int i = 0; i < THREAD_NUM_BIG; i++) {
		bigFileThread[i].join();
	}

	for (int i = 0; i < THREAD_NUM_SMALL; i++) {
		smallFileThread[i].join();
	}
}

void HandleDownload::DownloadSmallFile() {

	request new_req;

	while (1) {
		std::unique_lock<std::mutex> ul(SmallFileMtx);

		bool isTerminated = terminate.load();

		// wait if the queue is not empty
		SmallFile_cv.wait(ul, [this, isTerminated]() {
			return (!SmallFileRequest_q.isEmpty() || isTerminated);
		});

		if (isTerminated) {
			ul.unlock();
			break;
		}

		try {
			// extract the request from the queue and release the mutex 
			SmallFileRequest_q.removeRequest(new_req);
			ul.unlock();
		}
		catch (std::exception &e) {
			e.what();
		}

		// do stuff here 
	
		// decide if the opening of the file will be in asynchronous manner or sequential
		try {
			std::future<FileHandler> openFile = std::async(&HandleDownload::_openFile, this, new_req.fileName);
		} catch(FileOpenException &e) {

		}

	}

	// second port mechanism
	std::unique_lock<std::mutex> ul(SmallFileMtx_2);
	while (SmallFileRequest_q.isEmpty()) {
		
		SmallFileRequest_q.removeRequest(new_req);
		ul.unlock();

		// do stuff here

		ul.lock();
	}

	// release the lock after that the queue is empty
	ul.unlock();
}

void HandleDownload::DownloadBigFile() {

	request new_req;

	while (1) {
		std::unique_lock<std::mutex> ul(BigFileMtx);

		bool isTerminated = terminate.load();
		// wait if the queue is empty
		BigFile_cv.wait(ul, [this, isTerminated]() {
			return (!BigFileRequest_q.isEmpty() || isTerminated);
		});

		if (isTerminated) {
			ul.unlock();
			break;
		}

		BigFileRequest_q.removeRequest(new_req);
		ul.unlock();

		// do stuff here

		// open asynchronously the file
		try {
			std::future<FileHandler> async_open = std::async(&HandleDownload::_openFile, this, new_req.fileName);
		}
		catch (FileOpenException &e) {

		}



	}

	// second port for mechanism
	std::unique_lock<std::mutex> ul(BigFileMtx_2);
	while (BigFileRequest_q.isEmpty()) {

		BigFileRequest_q.removeRequest(new_req);
		ul.unlock();

		// do stuff here 

		ul.lock();
	}

	// release the lock when the queue is empty
	ul.unlock();
}

void HandleDownload::InsertSmallFileRequest(size_t fileSize, std::string filename, std::shared_ptr<TCPconnection> new_connection) {
	
	if (terminate.load() == false) {
		std::lock_guard<std::mutex> lg(SmallFileMtx);

		// insert in the queue for the small file
		SmallFileRequest_q.insertRequest(fileSize, filename, new_connection);

		// notify the all the thread that is arrived a new request of downlaod
		SmallFile_cv.notify_all();
	}
	else {
		// is not possible to insert new data
	}

}

void HandleDownload::InsertBigFileRequest(size_t fileSize, std::string filename, std::shared_ptr<TCPconnection> new_connection) {

	if (terminate.load() == false) {
		std::lock_guard<std::mutex> lg(BigFileMtx);

		// insert in the queue for the small file
		BigFileRequest_q.insertRequest(fileSize, filename, new_connection);

		// notify the all the thread that is arrived a new request of downlaod
		BigFile_cv.notify_all();
	}
	else {
		// is not possible to insert new data
	}

}


HandleDownload& HandleDownload::operator=(const HandleDownload& obj) {
	

}

FileHandler _openFile(std::string filename) {

	// if the filename is empty then throw an exception
	if (filename.empty()) {
		throw FileOpenException();
	}

	FileHandler file(filename);
	file.openFile();
}


