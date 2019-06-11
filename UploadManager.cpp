#include "pch.h"

bool UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketDispatcher packet_dispatcher) {

	try {
		std::cout << "Waiting for packet" << std::endl;
		ProtocolMessage packet = packet_dispatcher.receive_packet();
		packet.compute_packet_type();

		bool retry = true;

		while(retry) {
			if (packet.get_message_code() == protocol::err){
				if(packet.get_error_code() == protocol::err_packet_format || packet.get_error_code() == protocol::err_timeout) {
					retry = true;
					packet_dispatcher.send_packet(file_handler);
					packet = packet_dispatcher.receive_packet();
					packet.compute_packet_type();
				} else if(packet.get_error_code() == protocol::err_file) {
					std::cout << "Error with file information" << std::endl;
					file_request->connection_->close_connection();
					return false;
				} else {
					std::cout << "Unknown packet error" << std::endl;
					file_request->connection_->close_connection();
					return false;
				}
			} else
				break;
		}

		if (packet.get_message_code() == protocol::ok) {
			std::cout << "File sending..." << std::endl;

			if(file_request->connection_->send_file(file_handler.get_file_handle(), file_handler.get_file_size()) != 0) {
				std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << " Last error: " << GetLastError() << std::endl;
				file_request->connection_->close_connection();
				return false;
			}

			std::cout << "Aspetto di ricevere OK!" << std::endl;

			ProtocolMessage packet2 = packet_dispatcher.receive_packet();
			packet2.compute_packet_type();

			std::cout << "RICEVUTO: " << packet2.get_message_code() << std::endl;

			std::cout << "File sent!" << std::endl;
			file_request->connection_->close_connection();

			return true;

		}

		std::cout << "Received unknown packet!" << std::endl;
		file_request->connection_->close_connection();
		return false;

	}catch(SocketException &e) {
		std::cout << "Socket exception: " << e.get_error_code() << std::endl;
		return false;
	} catch (std::exception &e) {
		
		std::cout << "ECCEZIONE NEL FUTURE: " << e.what() << "                 " << GetLastError() << std::endl;
		return false;
	}
}
