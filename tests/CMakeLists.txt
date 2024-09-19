# CMakeLists.txt for the tests directory

cmake_minimum_required(VERSION 3.0)
project(ipc_tests C)

# Find the CMockA library
find_package(CMocka REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/../include)

# Link directories to the libsrc where the object files are located
link_directories(${PROJECT_SOURCE_DIR}/../lib)

# Add the test executable
add_executable(test_ipc_socket test_ipc_socket_new.c)

# Link against CMocka and the library that contains ipc_socket_create
target_link_libraries(test_ipc_socket cmocka pthread ipc_library)

# Register the test
enable_testing()
add_test(NAME test_ipc_socket COMMAND test_ipc_socket)
