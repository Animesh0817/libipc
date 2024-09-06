/**
 * @file ipc_socket.c
 * author Animesh0817 (mailtome.anni@gmail.com)
 * @brief Implementation of IPC using sockets.
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ipc_socket.h"
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


/**
 * @brief Initialize the IPC socket.
 *
 * @param handle Pointer to the IPC socket handle.
 * @return IPC_SUCCESS on success, IPC_FAILURE on failure.
 */
static int ipc_socket_init(ipc_handle_t *handle) {
    ipc_socket_t *sock = (ipc_socket_t *)handle;

    if (sock->is_server) {
        // Server: Bind and listen
        if (bind(sock->sockfd, (struct sockaddr *)&sock->addr, sizeof(sock->addr)) == -1) {
            close(sock->sockfd);
            return IPC_FAILURE;
        }
        if (listen(sock->sockfd, 5) == -1) {
            close(sock->sockfd);
            return IPC_FAILURE;
        }
    } else {
        // Client: Connect
        if (connect(sock->sockfd, (struct sockaddr *)&sock->addr, sizeof(sock->addr)) == -1) {
            close(sock->sockfd);
            return IPC_FAILURE;
        }
    }

    return IPC_SUCCESS;
}

/**
 * @brief Send a message through the IPC socket.
 *
 * @param handle Pointer to the IPC socket handle.
 * @param msg Pointer to the message to send.
 * @param len Length of the message.
 * @return IPC_SUCCESS on success, IPC_FAILURE on failure.
 */
static int ipc_socket_send(ipc_handle_t *handle, const void *msg, size_t len) {
    ipc_socket_t *sock = (ipc_socket_t *)handle;
    if (send(sock->sockfd, msg, len, 0) == -1) {
        return IPC_FAILURE;
    }
    return IPC_SUCCESS;
}

/**
 * @brief Receive a message from the IPC socket.
 *
 * @param handle Pointer to the IPC socket handle.
 * @param buf Buffer to store the received message.
 * @param len Length of the buffer.
 * @return IPC_SUCCESS on success, IPC_FAILURE on failure.
 */
static int ipc_socket_receive(ipc_handle_t *handle, void *buf, size_t len) {
    ipc_socket_t *sock = (ipc_socket_t *)handle;
    ssize_t bytes_received = recv(sock->sockfd, buf, len, 0);
    if (bytes_received == -1 || bytes_received == 0) {
        return IPC_FAILURE;
    }
    return IPC_SUCCESS;
}

/**
 * @brief Destroy the IPC socket.
 *
 * @param handle Pointer to the IPC socket handle.
 * @return IPC_SUCCESS on success, IPC_FAILURE on failure.
 */
static int ipc_socket_destroy(ipc_handle_t *handle) {
    ipc_socket_t *sock = (ipc_socket_t *)handle;
    if (close(sock->sockfd) == -1) {
        return IPC_FAILURE;
    }
    free(sock);
    return IPC_SUCCESS;
}

/**
 * @brief Accept a connection on a server socket.
 *
 * @return A new IPC socket handle for the accepted client connection, or NULL on failure.
 */
static ipc_handle_t *ipc_socket_accept(ipc_socket_t *server_sock) {
    ipc_socket_t *client_sock = (ipc_socket_t *)malloc(sizeof(ipc_socket_t));
    if (!client_sock) {
        return NULL;
    }

    socklen_t addrlen = sizeof(client_sock->addr);
    client_sock->sockfd = accept(server_sock->sockfd, (struct sockaddr *)&client_sock->addr, &addrlen);
    if (client_sock->sockfd == -1) {
        free(client_sock);
        return NULL;
    }

    client_sock->is_server = 0;
    client_sock->base.init = (int (*)(void *))ipc_socket_init;
    client_sock->base.send = (int (*)(void *, const void *, size_t))ipc_socket_send;
    client_sock->base.receive = (int (*)(void *, void *, size_t))ipc_socket_receive;
    client_sock->base.destroy = (int (*)(void *))ipc_socket_destroy;

    return (ipc_handle_t *)client_sock;
}

/**
 * @brief Create a new IPC socket handle.
 *
 * @param address IP address of the socket.
 * @param port Port of the socket.
 * @param is_server Flag to indicate if this is a server or client socket.
 * @return Pointer to the created IPC socket handle.
 */
ipc_handle_t *ipc_socket_create(const char *address, int port, int is_server) {
    ipc_socket_t *sock = (ipc_socket_t *)malloc(sizeof(ipc_socket_t));
    if (!sock) {
        return NULL;
    }
    
    // Create socket
    sock->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->sockfd == -1) {
        free(sock);
        return NULL;
    }
    
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &sock->addr.sin_addr) <= 0) {
        close(sock->sockfd);
        free(sock);
        return NULL;
    }

    sock->is_server = is_server;

    // Assign function pointers
    sock->base.init = (int (*)(void *))ipc_socket_init;
    sock->base.send = (int (*)(void *, const void *, size_t))ipc_socket_send;
    sock->base.receive = (int (*)(void *, void *, size_t))ipc_socket_receive;
    sock->base.destroy = (int (*)(void *))ipc_socket_destroy;
    sock->base.accept = is_server ? (ipc_handle_t *(*)(ipc_handle_t *))ipc_socket_accept : NULL;


    return (ipc_handle_t *)sock;
}
