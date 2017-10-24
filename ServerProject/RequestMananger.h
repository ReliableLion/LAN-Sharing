#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include "TCPconnection.h"
#include "DownloadManager.h"
#include "Message.hpp"
#include <memory>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#define MAX_THREADS 2

using namespace boost::asio;

class RequestManager {
private:
	std::thread t[MAX_THREADS];
	std::queue<std::shared_ptr<TCPconnection_server>> connection_pool;
	std::mutex mtx1;
	std::condition_variable cv;
	std::atomic<bool> terminate;

	DownloadManager d_man;
	
	void processRequest();
	void requestHandShake(std::shared_ptr<TCPconnection_server> conn);
	bool checkParameter(__int64 size, std::string filename);

public:
	RequestManager();
	~RequestManager();
	void addRequest(std::shared_ptr<TCPconnection_server> conn);
	void shutdown();
};

#endif // !REQUEST_MANAGER_H

