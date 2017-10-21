#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include "TCPconnection.h"
#include "HandleDownload.h"
#include "Message.h"
#include <memory>

using namespace boost::asio;

class RequestManager {
private:
	ConnectionPool c_pool;
	HandleDownload dw_manager;

public:
	RequestManager();
	~RequestManager();


};

#endif // !REQUEST_MANAGER_H

