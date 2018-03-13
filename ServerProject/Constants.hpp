#pragma once

#define MAXBUFL 1024
#define CHUNK 1460
#define UDP_PORT 1234
#define DEFAULT_LISTEN_PORT 1500
#define TIMEOUT 3
#define BIG_FILE_THREADS 3
#define SMALL_FILE_THREADS 2
#define FILE_SIZE_THRESHOLD 1024 * 1024		// 1MB
#define REQUEST_THREADS 4
#define MAX_REQUEST_ATTEMPTS 3
#define MAX_FILE_ATTEMPTS 5
#define TEMP_PATH std::string("C:\\users\\utentepc\\Appdata\\local\\temp")
#define QUEUE_MAX_ELEMENTS 1000
#define BUFF_LENGTH 1024			// 1KB
#define TIMEOUT_SEC 10				// 10 seconds
#define TIMEOUT_USEC 0				// 0 milliseconds

#define MAX_PORT 65535

/* UDP SECTION */
/**
* \brief UDP port used to which the UDP server is listening to
*/
#define UDP_PORT 1234
/**
* \brief Timeout used by the SELECT() when listening for UDP Discovery Messages
*/
#define DISCOVERY_TIMEOUT 3
/**
* \brief Message appended within an Hello Message
*/
#define HELLO_MSG  "MYUSERNAME "
/**
* \brief Message appended within a Discovery Message
*/
#define DISCOVERY_MSG "LAN-SHARING LOOKINGFOR"

#define USERNAME "Davide"
#define USERNAME_LENGTH 255
