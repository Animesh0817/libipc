cmake_minimum_required(VERSION 3.10)
project(libipc VERSION 1.0.0 LANGUAGES C)

# Set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set compiler flags for debugging
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0")

# Include the directory for the header files
include_directories(include)

# Source files
set(SOURCES
    libsrc/ipc.c
)

# Add the IPC library
add_library(ipc_library STATIC
    libsrc/ipc.c
    libsrc/ipc_socket.c
)

# Add the IPC library
add_library(ipc_library_shared SHARED
    libsrc/ipc.c
    libsrc/ipc_socket.c
)

# Add subdirectories
add_subdirectory(tests)
add_subdirectory(example)

# Doxygen documentation
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

