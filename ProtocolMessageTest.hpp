#pragma once

#include "PacketManager.hpp"
#include "Connection.hpp"


class ProtocolMessageTest {
public:
    ProtocolMessageTest() {}
	static void download_request(connection::conn_ptr connection);
};
