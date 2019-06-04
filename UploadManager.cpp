#include "UploadManager.hpp"
#include <iostream>

#include <chrono>
#include "Exceptions.hpp"
//#include <stdlib.h>     //for using the function sleep

#pragma comment(lib, "Mswsock.lib")

bool UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketDispatcher packet_dispatcher) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));
	try {
	std::cout << "Waiting for packet" << std::endl;
	ProtocolMessage packet = packet_dispatcher.receive_packet();
	packet.compute_packet_type();

	if (packet.get_message_code() == protocol::ok) {
		std::cout << "File sending..." << std::endl;

		auto start = std::chrono::high_resolution_clock::now();

		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

		std::cout << "hour before transmit " << file_request->file_name_ << ss.str() << std::endl;

		//if(!TransmitFile(file_request->connection_->get_handle_socket(), file_handler.get_file_handle(), file_request->file_size_, 0, nullptr, nullptr, TF_USE_DEFAULT_WORKER)) {
		//	std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << std::endl;
		//	file_request->connection_->close_connection();
		//	return false;
		//}

		if(file_request->connection_->send_file(file_handler.get_file_handle(), file_handler.get_file_size()) != 0) {
			std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << std::endl;
			file_request->connection_->close_connection();
			return false;
		}

		start = std::chrono::high_resolution_clock::now();

		now = std::chrono::system_clock::now();
		in_time_t = std::chrono::system_clock::to_time_t(now);

		ss.clear();
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

		std::cout << "hour after transmit " << file_request->file_name_ << ss.str() << std::endl;

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		std::cout << "Time taken by function: "
			<< duration.count() << " microseconds" << std::endl;

		std::cout << "Aspetto di ricevere OK!" << std::endl;

		ProtocolMessage packet2 = packet_dispatcher.receive_packet();
		packet2.compute_packet_type();

		std::cout << "RICEVUTO: " << packet2.get_message_code() << std::endl;

/*
		std::cout << "VADO IN SLEEP" << std::endl;
		Sleep(20000);*/

		std::cout << "File sent!" << std::endl;
		file_request->connection_->close_connection();

		return true;
	}

	std::cout << "The message wasn't ok" << std::endl;
	}catch(SocketException &e) {
		std::cout << "Socket exception: " << e.get_error_code() << std::endl;
		return false;
	} catch (std::exception &e) {
		std::cout << "ECCEZIONE NEL FUTURE: " << e.what() << "                 " << GetLastError() << std::endl;
	}
	return false;
}
