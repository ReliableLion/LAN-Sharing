#include "DownloadManager.h"

// when the costructor of this object is invoked the threads are createsd and go into sleep mode
void DownloadManager::setupDownloader() {
	terminate.store(false);

	for (int i = 0; i < THREAD_NUM_BIG; i++) {
		bigFileThread[i] = std::thread(&DownloadManager::DownloadBigFile, this);
	}

	for (int i = 0; i < THREAD_NUM_SMALL; i++) {
		smallFileThread[i] = std::thread(&DownloadManager::DownloadSmallFile, this);
	}
}

// define a method to exit 
DownloadManager::~DownloadManager() {
	exitDownloader();
}

void DownloadManager::exitDownloader() {
	terminate.store(true);

	for (int i = 0; i < THREAD_NUM_BIG; i++) {
		bigFileThread[i].join();
	}

	for (int i = 0; i < THREAD_NUM_SMALL; i++) {
		smallFileThread[i].join();
	}
}

void DownloadManager::DownloadSmallFile() {
	dwld_request new_req;

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
			SmallFileRequest_q.popElement(new_req);
			ul.unlock();
		}
		catch (std::exception &e) {
			e.what();
		}

		// do stuff here 

	}

	// second port mechanism
	std::unique_lock<std::mutex> ul(SmallFileMtx_2);
	while (SmallFileRequest_q.isEmpty()) {
		
		SmallFileRequest_q.popElement(new_req);
		ul.unlock();

		// do stuff here

		ul.lock();
	}

	// release the lock after that the queue is empty
	ul.unlock();
}

void DownloadManager::DownloadBigFile() {

	dwld_request new_req;
	std::string fileName;
	size_t fileSize;
	std::shared_ptr<TCPconnection_server> conn;
	std::future<FileHandler> async_open;

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

		BigFileRequest_q.popElement(new_req);
		ul.unlock();

		// do stuff here

		fileName = new_req.fileName;
		fileSize = new_req.fileSize;
		conn = new_req.connection;

		_downloadFile(fileName, fileSize, conn);

	}

	// second port for mechanism
	std::unique_lock<std::mutex> ul(BigFileMtx_2);
	while (BigFileRequest_q.isEmpty()) {

		BigFileRequest_q.popElement(new_req);
		ul.unlock();

		// do stuff here 

		fileName = new_req.fileName;
		fileSize = new_req.fileSize;
		conn = new_req.connection;

		_downloadFile(fileName, fileSize, conn);

		ul.lock();
	}

	// release the lock when the queue is empty
	ul.unlock();
}

void DownloadManager::InsertSmallFileRequest(size_t fileSize, std::string filename, std::shared_ptr<TCPconnection_server> new_connection) {
	
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

void DownloadManager::InsertBigFileRequest(size_t fileSize, std::string filename, std::shared_ptr<TCPconnection_server> new_connection) {

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

void DownloadManager::_downloadFile(std::string filename, size_t size, std::shared_ptr<TCPconnection_server> conn) {
	// variables used to perform a reading from network
	size_t downloaded_bytes, remaining_bytes;
	buffer_type read_buffer;
	std::shared_ptr<buffer_type> buffer_ptr = std::make_shared<buffer_type>(read_buffer);

	//definition of the file handler
	FileHandler original_file;
	FileHandler temp_file;

	try {
		// varaibles used to manage the file life-cycle
		// TODO	remember to set the path for the file
		original_file = FileHandler(filename, "");
		temp_file = FileHandler(filename, TEMP_PATH);

		// open asynchronously the two files
		file1 = std::async(&FileHandler::openFile, &original_file);
		file2 = std::async(&FileHandler::openFile, &temp_file);
		
		//wait to open the two files
		file1.wait();
		file2.wait();
	}
	catch (FileOpenException &e) {
		std::cerr << e.what() << std::endl;
		/*original_file.closeFile();
		original_file.removeFile();
		temp_file.closeFile()*/
	}

	try {
		// begin to read the data from the connection 
		remaining_bytes = size;
		while (remaining_bytes > 0) {
			downloaded_bytes = conn->readDataChunks(buffer_ptr);
			temp_file.writeData(buffer_ptr, downloaded_bytes);
			remaining_bytes -= downloaded_bytes;
		}
	}
	// TODO write the correct exception
	catch (TCPreadException &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "error: " << conn->getError() << std::endl;
		// close and remove the file
		original_file.closeFile();
		temp_file.closeFile();
		original_file.removeFile();
		temp_file.removeFile();
		//close the connection
		conn->closeConnection();
	}
	catch (FileWriteException &e) {
		//TODO add the file write exception handler
	}
	
	// store the temp file in the destination
	if (original_file.copyFile(temp_file)) {
		//TODO	the copy has gone good
		temp_file.closeFile();
		original_file.closeFile();
		temp_file.removeFile();

		//close the connection
		conn->closeConnection();
	}
	else {
		//TODO	if the copy has gone bad, repeate the copy
	}

}