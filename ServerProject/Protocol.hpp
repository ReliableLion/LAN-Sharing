#pragma once
#include "stdafx.h"
#include <map>

namespace protocol {

	class MessageType {
	private:
		const std::map<std::string, int> messageType = {
			std::make_pair(std::string("SEND"), 0),
			std::make_pair(std::string("+OK "), 1),
			std::make_pair(std::string("-ERR"), 2)
		};

		const std::map<std::string, int> errorType = {

		};

	public:
		const enum TYPE {
			OK,
			SEND,
			ERR,
			UNDEFINED
		};

		const enum ERROR_TYPE {

		};

	public:
		int getMessageType(std::string msgType) {

			if (messageType.at[msgType] == SEND) {
				return SEND;
			}
			else
				if (messageType.at[msgType] == ERR) {
					return ERR;
				}
				else
					if (messageType.at[msgType] == OK) {
						return OK;
					}
					else {
						return UNDEFINED;
					}

		}

		int getErrorType(std::string errString) {
			return 0;
		}
	};

	class Requi



};