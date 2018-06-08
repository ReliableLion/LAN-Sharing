#pragma once

#include "Constants.hpp"
#include <map>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

namespace udp_service {

    std::string get_client_address(struct sockaddr_in *client_address_ptr);

    class UdpClient {

    public:

        UdpClient();

        bool get_adapter();

        void get_server_info(std::string address, std::string port);

        int send_datagram(std::string buff) const;

        int receive_datagram();

        void send_broadcast(const char *message);

        map<string, string> get_online_users();

        ~UdpClient();

    private:
        int sock_;
        char buffer_[MAXBUFL] = "";
        char server_address_[INET_ADDRSTRLEN];
        char client_address_[INET_ADDRSTRLEN] = "";
        int server_port_;
        struct sockaddr_in broadcast_address_, server_address_struct_;

    };

    class UdpServer {

    public:
        UdpServer();

        int send_datagram(const char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;

        socklen_t receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len) const;

        ~UdpServer();

    private:
        int server_sock_;
        char buffer_[MAXBUFL] = "";
        char server_address_[INET_ADDRSTRLEN];

    };
}
