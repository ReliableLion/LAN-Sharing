//
// Ping.cpp
// ~~~~~~~~
//	Based on Christopher M. Kohlhoff (chris at kohlhoff dot com) version
//

#include "Ping.h"

void Pinger::start_send() {

	std::string body("\"Hello!\" from a LAN-Sharing peer.");

	// Create an ICMP header for an echo request.
	icmp_header echo_request;
	echo_request.type(icmp_header::echo_request);
	echo_request.code(0);
	echo_request.identifier(get_identifier());
	echo_request.sequence_number(++sequence_number_);
	compute_checksum(echo_request, body.begin(), body.end());

	// Encode the request packet.
	boost::asio::streambuf request_buffer;
	std::ostream os(&request_buffer);
	os << echo_request << body;

	// Send the request.
	time_sent_ = posix_time::microsec_clock::universal_time();
	socket_.send_to(request_buffer.data(), destination_);

	// Wait up to two seconds for a reply.
	num_replies_ = 0;
	timer_.expires_at(time_sent_ + posix_time::seconds(2));
	timer_.async_wait(boost::bind(&Pinger::handle_timeout, this));
}

void Pinger::handle_timeout()
{
	if (num_replies_ == 0)
		std::cout << "Request timed out" << std::endl;

	// Requests must be sent no less than one second apart.
	timer_.expires_at(time_sent_ + posix_time::seconds(1));
	timer_.async_wait(boost::bind(&Pinger::start_send, this));
}

void Pinger::start_receive()
{
	// Discard any data already in the buffer.
	reply_buffer_.consume(reply_buffer_.size());

	// Wait for a reply. We prepare the buffer to receive up to 64KB.
	socket_.async_receive(reply_buffer_.prepare(65536),
		boost::bind(&Pinger::handle_receive, this, _2));
}

void Pinger::handle_receive(std::size_t length)
{
	// The actual number of bytes received is committed to the buffer so that we
	// can extract it using a std::istream object.
	reply_buffer_.commit(length);

	// Decode the reply packet.
	std::istream is(&reply_buffer_);
	ipv4_header ipv4_hdr;
	icmp_header icmp_hdr;
	is >> ipv4_hdr >> icmp_hdr;

	// We can receive all ICMP packets received by the host, so we need to
	// filter out only the echo replies that match the our identifier and
	// expected sequence number.
	if (is && icmp_hdr.type() == icmp_header::echo_reply
		&& icmp_hdr.identifier() == get_identifier()
		&& icmp_hdr.sequence_number() == sequence_number_)
	{
		// If this is the first reply, interrupt the five second timeout.
		if (num_replies_++ == 0)
			timer_.cancel();

		// Print out some information about the reply packet.
		posix_time::ptime now = posix_time::microsec_clock::universal_time();
		std::cout << length - ipv4_hdr.header_length()
			<< " bytes from " << ipv4_hdr.source_address()
			<< ": icmp_seq=" << icmp_hdr.sequence_number()
			<< ", ttl=" << ipv4_hdr.time_to_live()
			<< ", time=" << (now - time_sent_).total_milliseconds() << " ms"
			<< std::endl;
	}

	start_receive();
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: ping <host>" << std::endl;
#if !defined(BOOST_WINDOWS)
			std::cerr << "(You may need to run this program as root.)" << std::endl;
#endif
			return 1;
		}

		boost::asio::io_service io_service;
		Pinger p(io_service, argv[1]);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}