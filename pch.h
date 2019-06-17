// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

typedef void (*EXCEPTION_CALLBACK)();
typedef void (*DISCOVERY_MANAGED_CALLBACK)(const char* ipAddress,const char* username,const char* avatar);
typedef bool (*ACCEPT_CALLBACK)();
typedef void (*PROGRESS_BAR_CALLBACK)(const char *id, int progress);


#include "ManagedCallback.hpp"

// add headers that you want to pre-compile here
#include "framework.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <utility>

#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <mswsock.h>
#pragma comment(lib, "Mswsock.lib")

#pragma comment(lib, "ws2_32.lib")

#include "Constants.hpp"
#include "Utils.hpp"
#include "User.hpp"
#include "UserRequest.hpp"

#include "ConcurrentQueue.hpp"
#include "SocketBuffer.hpp"
#include "Exceptions.hpp"
#include "Connection.hpp"
#include "ConcurrentStreamPrint.hpp"
#include "Protocol.hpp"
#include "WindowsFileHandler.hpp"
#include "Message.hpp"
#include "UDPService.hpp"
#include "Discovery.hpp"
#include "FileHandler.hpp"
#include "PacketDispatcher.hpp"
#include "DownloadManager.hpp"
#include "HandshakeManager.hpp"
#include "RequestHandler.hpp"
#include "UploadManager.hpp"
#include "Server.hpp"


#endif //PCH_H
