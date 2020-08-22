/**
 * ALEFT PROJECT
 * 
 * @author Alexandre E.
 * @author Lev M.
 * @date August 2020
 * 
 * @note This program is a part of the ALEFT Project.
 *       It's a naive file transfert program, which allows
 *       two users to transfer a file to each other.
 * 
 *       This program simply listens through the port 11037.
 * */
#ifndef __RECEIVER__
#define __RECEIVER__
#include <unistd.h>

/*
In order for this receiver to understand the incoming file,
the packet must have this structure:

[HEADER]
    FILENAME_LEN Bytes of filename
        e.g. "h e l l o . t x t \0 ... \0 "
              1 2 3 4 5 6 7 8 9 10 ... 128
    
    FILESIZE_LEN Bytes of file size
        e.g. "      2048" = 2048 Bytes

[FILE CONTENT]
    Everything that follows the header is the sent file.
    The number of bytes from this section is known
    thanks to the size written in the HEADER.
    (e.g. 2048 Bytes according to the above example)

[PACKET EXAMPLE]
    "hi.txt\0...\0         7bonjour"
    the header is: "hi.txt\0...\0         7" where
    the "\0...\0" is a series of 122 '\0'

    the file content is: "bonjour"
*/

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

#define PORT "11037"
#define BUF_SIZE 1024

#define FILENAME_LEN 128
#define FILESIZE_LEN 10

typedef int SOCKET;

/**
 * Creates the program's socket.
 * 
 * @return the socket's file descriptor or -1 if an error has occured
 */
SOCKET create_socket();

/**
 * Performs a casting to get the sin_addr from
 * the sockaddr_in version of the given sockaddr struct
 * 
 * @param sa sockaddr struct from which read the sin_addr
 * */
void* get_in_addr(struct sockaddr* sa);

/**
 * Waits for someone to connect through sockfd.
 * 
 * @return the connected sockfd if a new connection has been made
 *         -1 if an error has occured
 */
SOCKET listen_sender(SOCKET sockfd);

/**
 * Once the connecion is made, the transfer can begin and this
 * function awaits for the file.
 * 
 * @param senderSocket socketfd of the sender
 * 
 * @return EXIT_SUCCESS if the transfer was successful
 *         EXIT_FAILURE if an error has occured
 */
int start_transfer(SOCKET senderSocket);

/**
 * Listens for the file header.
 * 
 * @arg sockfd : socket to listen to
 * @arg headerSize : address of a variable in which to write the size
 * 
 * @return the received header if it has been completely received
 *         NULL if the connection was broken
 */
char* recvHeader(SOCKET sockfd, size_t* headerSize);

/**
 * Puts the file size from the header into size
 * 
 * @param header header from which decode the file size
 * @param size address of the size variable to use
 */
void decode_fileSize(char* header, size_t* size);

/**
 * Puts the filename from the header into fileName
 * 
 * @param header header from which decode the fileName
 * @param fileName str in which write the fileName
 */
void decode_fileName(char* header, char* fileName);

/**
 * Checks the format of the header
 * 
 * @param filename file name
 * @param fileSizeStr file size in str format
 * 
 * @return true if the format of both args is fine
 *         false if it is not
 */
bool check_header(char* filename, char* fileSizeStr);

/**
 * Listens to sockfd to receive the file
 * 
 * @param sockfd sender's socket descriptor
 * @param rawfile pointer to the str in which write the received file
 * @param recvBytesNb number of already received bytes
 * @param fileSize size of the file awaited
 * 
 * If an error occurs, (*rawfile) is set to NULL and the memory is freed.
 */
void recvFile(SOCKET sockfd, char** rawfile, size_t recvBytesNb, size_t fileSize);

/**
 * Saves the string contained in rawfile in a file "fileName".
 * 
 * @param rawfile string to save into a file
 * @param fileName name of the file
 * @param fileSize size of rawfile
 */
int copy_str_to_file(char* rawfile, char* fileName, size_t fileSize);

#endif // __RECEIVER__