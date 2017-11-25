#pragma once

/**
 * \brief Buffer length used for the UDP messages
 */
#define MAXBUFL 1024
/**
 * \brief Minimum unit of payload within a TCP packet
 */
#define CHUNK 1460
 /**
 * \brief Timeout used by the SELECT()
 */
#define TIMEOUT 3

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

/* FILE SECTION */
#define MAX_FILE_ATTEMPTS 5