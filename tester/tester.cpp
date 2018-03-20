// tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
	
#include "Message.hpp"
#include "FileManagerWin32.h"

#include <windows.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib") //Required for WinSock

int main()
{
	LPFILETIME file_time;
	__int64 file_size = 0;
	tstring file_name = TEXT("C:\\Users\\utente pc\\Documents\\C.pdf");

	FileManager file1(file_name);

	if(file1.openFile())
	{
		if(file1.getFileTimeStamp() && file1.getFileSize())
		{
			file_time = file1.getLastModified();
			file_size = file1.getSize();
			// since the file time is lpfiletime dereference it
			ProtocolMessage protocol_message(file_size, *file_time, file_name);
			// check if the message is correclty created
			protocol_message.prepare_outgoing_packet();
		}
	}

    return 0;
}

