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
 * @return Pointer to the created IPC handle.
 */
ipc_handle_t *ipc_create(void) {
    ipc_handle_t *handle = (ipc_handle_t *)malloc(sizeof(ipc_handle_t));
    if (!handle) {
        return NULL;
    }
    return handle;
}

/**
 * @brief Destroy an IPC handle.
 * 
 * @param handle Pointer to the IPC handle to destroy.
 */
void ipc_destroy(ipc_handle_t *handle) {
    if (handle) {
        free(handle);
    }
}
