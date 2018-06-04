#include "ProtocolMessageTest.hpp"
#include "Exceptions.hpp"

#include <iostream>

void ProtocolMessageTest::download_request(const connection::conn_ptr connection) {
	const PacketManager packet_manager(connection);

    try {
            ProtocolMessage packet = packet_manager.receive_packet();
            packet.compute_packet_type();

            switch (packet.get_message_code()) {

                case protocol::undefined : {
                    packet_manager.send_error(protocol::err_1);
                }
                    break;

                case protocol::ok : {

                    if (packet.compute_request()) {
	                    auto request = packet.get_message_request();
                    } else {
                        std::cout << "the request is not valid" << std::endl;
                    }

                }
                    break;

                default:
                    break;
            }


        std::cout << "request received correclty" << std::endl;

    } catch (TimeoutException &te) {
        std::cout << "server reached the timeout, close the connection" << std::endl;
        connection->close_connection();
    } catch (SocketException &se) {
        std::cout << "server encourred in a socket exception, close the connection" << std::endl;
        connection->close_connection();
    } catch (ConnectionCloseException &ce) {
        std::cout << "the connection has been closed by the peer" << std::endl;
    }

    connection->close_connection();
}
