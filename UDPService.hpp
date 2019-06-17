#pragma once
#include "pch.h"
#include <list>
#include <string>
#include <atomic>

using namespace std;

namespace udp_service {

    std::string get_client_address(struct sockaddr_in *client_address_ptr);

    class UdpClient {

    public:

        UdpClient();

        list<std::string> get_adapter();

		void set_client_address(std::string ip_address);

        void get_server_info(std::string address, std::string port);

        void send_broadcast(const char *message);

		bool is_ip_in_range(const std::string ip_address);

		list<string> get_addresses() {return my_addresses_;}

        ~UdpClient();

    private:
        int sock_;
        char buffer_[MAXBUFL] = "";
        char server_address_[INET_ADDRSTRLEN];
        int server_port_;
        struct sockaddr_in broadcast_address_, server_address_struct_;
		list<string> my_addresses_;

    };

    class UdpServer {

    public:
        UdpServer();

		void start_server();

		std::string print() { return std::to_string(server_sock_);}

		void stop();

        int send_datagram(const char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) ;

        socklen_t receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len);

        ~UdpServer();

    private:
        int server_sock_{};
        char buffer_[MAXBUFL] = "";
        char server_address_[INET_ADDRSTRLEN]{};

		atomic<bool> exit_udp;
    };
}
