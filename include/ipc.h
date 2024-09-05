/**
 * @file ipc.h
 * author Animesh0817 (mailtome.anni@gmail.com)
 * @brief Defines the common interface and base structure for IPC mechanisms.
 *
 * This file provides the interface that all IPC mechanisms should implement.
 * It defines the `ipc_handle_t` structure, which serves as a common base
 * for different IPC mechanisms, and a set of function pointers to be
 * implemented by each IPC type.
 */

#ifndef IPC_H
#define IPC_H
#define IPC_SUCCESS 0
#define IPC_FAILURE -1

#include <stddef.h>

/**
 * @struct ipc_handle_t
 * @brief The base structure for all IPC mechanisms.
 *
 * This structure provides a unified interface for different IPC mechanisms.
 * It contains function pointers that point to the implementation of the
 * corresponding IPC operations.
 */
typedef struct ipc_handle_t {
    
    /**
     * @brief Initializes the IPC mechanism.
     *
     * This function initializes the IPC mechanism, setting up any required
     * resources or configurations.
     *
     * @param ctx context pointer for different IPCs to the function pointers
     * @return 0 on success, or a negative error code on failure.
     */
    int (*init)(void *ctx);

    /**
     * @brief Sends data through the IPC mechanism.
     *
     * This function sends data using the IPC mechanism. The specific behavior
     * depends on the IPC mechanism being used (e.g., sending a message, writing
     * to shared memory, signaling, etc.).
     *
     * @param ctx context pointer for different IPCs to the function pointers
     * @param data Pointer to the data to be sent.
     * @param size Size of the data to be sent in bytes.
     * @return 0 on success, or a negative error code on failure.
     */
    int (*send)(void *ctx,const void *data, size_t size);

    /**
     * @brief Receives data from the IPC mechanism.
     *
     * This function receives data using the IPC mechanism. The specific behavior
     * depends on the IPC mechanism being used (e.g., receiving a message, reading
     * from shared memory, waiting for a signal, etc.).
     *
     * @param ctx context pointer for different IPCs to the function pointers
     * @param buffer Pointer to the buffer where received data will be stored.
     * @param size Size of the buffer in bytes.
     * @return The number of bytes received on success, or a negative error code on failure.
     */
    int (*receive)(void *ctx, void *buffer, size_t size);

    /**
     * @brief Destroys the IPC mechanism.
     *
     * This function cleans up any resources allocated by the IPC mechanism,
     * effectively terminating its use. It should be called when the IPC mechanism
     * is no longer needed.
     *
     * @param ctx context pointer for different IPCs to the function pointers
     * @return 0 on success, or a negative error code on failure.
     */
    int (*destroy)(void *ctx);

    /**
     * @brief Accepts a new connection on a server socket.
     *
     * This function pointer is used for server socket IPC to accept incoming client connections.
     * When implemented, it should accept a new connection from the server socket, creating
     * and returning a new IPC handle for the client connection.
     *
     * @param server_handle Pointer to the server IPC handle. This handle represents the server
     *        socket that is listening for incoming connections.
     *
     * @return A pointer to a new `ipc_handle_t` representing the client connection on success.
     *         Returns `NULL` if the connection could not be accepted or an error occurred.
     */
    struct ipc_handle_t *(*accept)(struct ipc_handle_t *server_handle);

} ipc_handle_t;

ipc_handle_t *ipc_create();
void ipc_destroy(ipc_handle_t *handle);

#endif // IPC_H
