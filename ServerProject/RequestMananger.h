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

	std::shared_ptr<DownloadManager> d_man_ptr;
	
	void _processRequest();
	void _requestHandShake(std::shared_ptr<TCPconnection_server> conn);
	bool _checkParameter(__int64 size, std::string filename);

public:
	RequestManager(std::shared_ptr<DownloadManager> d_man_ptr);
	~RequestManager();
	void addRequest(std::shared_ptr<TCPconnection_server> conn);
	void shutdown();
	void closeConnections();
};

#endif // !REQUEST_MANAGER_H

