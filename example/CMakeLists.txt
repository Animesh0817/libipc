# CMakeLists.txt for examples directory

# Include the IPC library
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_BUILD_TYPE Debug)

# Add the executable for simple IPC socket server example
add_executable(example_socket_server ipc_socket_server.c)
add_executable(example_socket_client ipc_socket_client.c)

# Link the IPC library and pthread for threading support
target_link_libraries(example_socket_server PRIVATE ipc_library pthread)
target_link_libraries(example_socket_client PRIVATE ipc_library pthread)

# Add this example as an installable target (optional)
install(TARGETS example_socket_server DESTINATION bin)
install(TARGETS example_socket_client DESTINATION bin)
