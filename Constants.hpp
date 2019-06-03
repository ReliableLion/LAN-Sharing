#pragma once

/**
 * \brief Buffer length used for the UDP messages
 */
#define MAXBUFL 1024
/**
 * \brief Minimum unit of payload within a TCP packet
 */
#define CHUNK 1024 * 1024
 /**
 * \brief Timeout used by the SELECT()
 */
#define TIMEOUT 120

#define TIMEOUT_SEC 120				// 10 seconds
#define TIMEOUT_USEC 0				// 0 milliseconds

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
#define DISCOVERY_MSG "LAN-SHARING LOOKING FOR"
/**
 * \brief Network adapter name used to send datagram
 */
#define NETWORK_ADAPTER_NAME "{4B80AC2C-32A7-4C94-B45A-D714A8EA5B76}"

#define USERNAME "Davide"
#define USERNAME_LENGTH 255

/* FILE SECTION */
#define BIG_FILE_THREADS 3
#define SMALL_FILE_THREADS 2
#define FILE_SIZE_THRESHOLD 1024 * 1024	* 600	// 1MB
#define REQUEST_THREADS 4
#define MAX_REQUEST_ATTEMPTS 3
#define MAX_FILE_ATTEMPTS 5
#define TEST_PATH std::string("C:\\Users\\utente pc\\test_project")
#define TEMP_PATH std::string("C:\\Users\\utente pc\\AppData\\Local\\Temp")
#define QUEUE_MAX_ELEMENTS 1000
#define MAX_FILENAME_LENGTH 256

/* TCP SECTION */
#define DEFAULT_LISTEN_PORT 1500

/* COMPILATION SWITCH */
// #define APP_DEBUG 
// #define FILE_HANDLER_OUTPUT 