#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <istream>
#include <iostream>
#include <ostream>

#include "ICMPHeader.h"
#include "IPv4Header.h"

using boost::asio::ip::icmp;
using boost::asio::deadline_timer;
namespace posix_time = boost::posix_time;

class Pinger {

public:
	Pinger(boost::asio::io_service& io_service, const char* destination)
		: resolver_(io_service), socket_(io_service, icmp::v4()),
		timer_(io_service), sequence_number_(0), num_replies_(0) {

		icmp::resolver::query query(icmp::v4(), destination, "");
		destination_ = *resolver_.resolve(query);

		start_send();
		start_receive();
	}

private:
	icmp::resolver resolver_;
	icmp::endpoint destination_;
	icmp::socket socket_;
	deadline_timer timer_;
	unsigned short sequence_number_;
	posix_time::ptime time_sent_;
	boost::asio::streambuf reply_buffer_;
	std::size_t num_replies_;

	void start_send();
	void handle_timeout();
	void start_receive();
	void handle_receive(std::size_t length);

	static unsigned short get_identifier() {
#if defined(BOOST_WINDOWS)
				return static_cast<unsigned short>(::GetCurrentProcessId());
#else
				return static_cast<unsigned short>(::getpid());
#endif
	};

};