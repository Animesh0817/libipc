/**
  * @file ipc_socket.h
  * author Animesh0817 (mailtome.anni@gmail.com)
  * @brief IPC using sockets.
  */

 #ifndef IPC_SOCKET_H
 #define IPC_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ipc.h"
#include <errno.h>

/**
  * A Structure that will hold the following:
  * Base IPC handle structure
  * File descriptor for the socket
  * Socket address information
  * Flag to indicate if this is a server or client socket.
  */
 typedef struct {
     ipc_handle_t base; /**< Base IPC handle structure. */
     int sockfd; /**< File descriptor for the socket. */
     struct sockaddr_in addr; /**< Socket address information. */
     int is_server; /**< Flag to indicate if this is a server or client socket. */
 } ipc_socket_t;

 ipc_handle_t *ipc_socket_create(const char *address, int port, int is_server);

 #endif // IPC_SOCKET_H
