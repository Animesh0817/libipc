/**
  * @file ipc_socket.h
  * author Animesh0817 (mailtome.anni@gmail.com)
  * @brief IPC using sockets.
  */

 #ifndef IPC_SOCKET_H
 #define IPC_SOCKET_H

 #include "ipc.h"

 ipc_handle_t *ipc_socket_create(const char *address, int port, int is_server);

 #endif // IPC_SOCKET_H
