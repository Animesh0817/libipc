/**
 * @file ipc_socket_server.c
 * author Animesh0817 (mailtome.anni@gmail.com)
 * @brief Simple IPC socket server using libipc framework for TCP-based communication.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_socket.h"
#include "ipc.h"

#define DEFAULT_IP "0.0.0.0"
#define PORT 8080
#define BUFFER_SIZE 1024

/**
 * @brief Handles communication with the client.
 *
 * @param client_sock The IPC socket handle for the connected client.
 */
int handle_client(ipc_handle_t *client_sock) {
    char buffer[BUFFER_SIZE] = {0};

    // Receive message from the client using ipc_socket_receive
    if (client_sock->receive(client_sock, buffer, sizeof(buffer)) != IPC_SUCCESS) {
        printf("Failed to receive data from client.\n");
        return IPC_FAILURE;
    }

    printf("Received message: %s\n", buffer);

    // Respond to the client using ipc_socket_send
    const char *response = "Hello from server!";
    if (client_sock->send(client_sock, response, strlen(response)) != IPC_SUCCESS) {
        printf("Failed to send data to client.\n");
    }

    // Destroy the IPC socket for the client connection
    client_sock->destroy(client_sock);
    return IPC_SUCCESS;
}

/**
 * @brief Function to perform Server side of IPC via LIBIPC
 */
int socket_server_example() {
        // Create a listening IPC socket handle using libipc
    ipc_handle_t *server_socket = ipc_socket_create(DEFAULT_IP, PORT, 1); // 1 for server
    if (server_socket == NULL) {
        printf("Failed to create server socket.\n");
        return IPC_FAILURE;
    }

    // Initialize the server socket (bind and listen)
    if (server_socket->init(server_socket) != IPC_SUCCESS) {
        printf("Failed to initialize server socket.\n");
        server_socket->destroy(server_socket);
        return IPC_FAILURE;
    }

    printf("Server listening on port %d\n", PORT);

    // Loop to accept and handle client connections
    while (1) {
        ipc_handle_t *client_socket = server_socket->accept((ipc_handle_t *)server_socket);
        if (client_socket == NULL) {
            //printf("Failed to accept client connection.\n");
            continue;
        }

        printf("Client connected\n");

        // Handle communication with the connected client
        handle_client(client_socket);
    }

    // Destroy the server socket (unreachable code)
    server_socket->destroy(server_socket);
    return IPC_SUCCESS;
}

/**
 * @brief Main Driver function
 */
int main() {
    int ret = -1;
    printf("Starting IPC Library Example Socket Server Application...\n");

    // Demonstrate each IPC mechanism
    ret = socket_server_example();
    printf("IPC Library Example Socket Server Application completed. Return is %d\n", ret);
    return ret;
}
