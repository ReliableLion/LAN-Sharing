#pragma once

#include "PacketManager.hpp"
#include "Exceptions.hpp"
#include "Connection.hpp"


class ProtocolMessageTest {
public:
    ProtocolMessageTest() {}
    void download_request(connection::conn_ptr connection);
};
