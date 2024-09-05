/**
 * @file ipc_client.c
 * author Animesh0817 (mailtome.anni@gmail.com)
 * @brief Simple IPC socket client using libipc framework for TCP-based communication.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_socket.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

/**
 * @brief Main function for the client.
 */
int socket_client_example() {
    // Create a client IPC socket handle using libipc
    ipc_handle_t *client_socket = ipc_socket_create(SERVER_IP, PORT, 0);  // Pass 0 for client
    if (client_socket == NULL) {
        printf("Failed to create client socket.\n");
        return IPC_FAILURE;
    }

    // Initialize the client socket
    if (client_socket->init(client_socket) != IPC_SUCCESS) {
        printf("Failed to initialize client socket.\n");
        client_socket->destroy(client_socket);
        return IPC_FAILURE;
    }

    // Send message to the server
    const char *message = "Hello from client!";
    if (client_socket->send(client_socket, message, strlen(message)) != IPC_SUCCESS) {
        printf("Failed to send data to server.\n");
        client_socket->destroy(client_socket);
        return IPC_FAILURE;
    }

    // Receive response from the server
    char buffer[BUFFER_SIZE] = {0};
    if (client_socket->receive(client_socket, buffer, sizeof(buffer)) != IPC_SUCCESS) {
        printf("Failed to receive data from server.\n");
    } else {
        printf("Received message: %s\n", buffer);
    }

    // Destroy the client socket
    client_socket->destroy(client_socket);

    return IPC_SUCCESS;
}

 int main() {
     int ret = -1;
     printf("Starting IPC Library Example Socket Client Application...\n");

     // Demonstrate each IPC mechanism
     ret = socket_client_example();
     printf("IPC Library Example Socket Client Application completed. Return is %d\n", ret);
     return ret;
 }
