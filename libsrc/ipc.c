/**
 * @file ipc.c
 * author Animesh0817 (mailtome.anni@gmail.com)
 * @brief Implementation of base IPC functions.
 */

#include "ipc.h"
#include <stdlib.h>

/**
 * @brief Create a new IPC handle.
 *
 * This function allocates memory for a new Inter-Process Communication (IPC) handle
 * and returns a pointer to it. The IPC handle is an abstract representation
 * used to manage different types of IPC mechanisms (e.g., sockets, shared memory, etc.).
 *
 * This function does not initialize the handle beyond basic memory allocation.
 * The user is responsible for assigning or initializing the relevant function pointers
 * (e.g., init, send, receive, destroy) after creation.
 *
 * @return Pointer to the created IPC handle. Returns `NULL` if memory allocation fails.
 *
 * @note The returned handle must be freed using `ipc_destroy` to avoid memory leaks.
 *
 * Example usage:
 * @code
 * ipc_handle_t *handle = ipc_create();
 * if (handle == NULL) {
 *     // Handle memory allocation failure
 * }
 * @endcode
 */
ipc_handle_t *ipc_create(void) {
    ipc_handle_t *handle = (ipc_handle_t *)malloc(sizeof(ipc_handle_t));
    if (!handle) {
        return NULL;
    }
    return handle;
}

/**
 * @brief Destroy an IPC handle and release allocated memory.
 *
 * This function frees the memory associated with a given IPC handle.
 * It is the responsibility of the caller to ensure that the handle is no longer
 * in use when this function is called, as calling this function will deallocate
 * the memory and invalidate the pointer.
 *
 * @param handle Pointer to the IPC handle to destroy. If the pointer is `NULL`,
 * this function does nothing.
 *
 * @note This function only frees the memory allocated for the IPC handle. If the handle
 * manages additional resources (e.g., file descriptors, network sockets), those must be
 * closed or released prior to calling `ipc_destroy`.
 *
 * Example usage:
 * @code
 * ipc_handle_t *handle = ipc_create();
 * if (handle != NULL) {
 *     // Perform IPC operations
 *     ipc_destroy(handle);  // Clean up once done
 * }
 * @endcode
 */
void ipc_destroy(ipc_handle_t *handle) {
    if (handle) {
        free(handle);
    }
}
