/**
 * @file test_ipc_socket.c
 * @brief Unit tests for ipc_socket.c using CMockA, adapted to use libipc function pointers.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ipc_socket.h"
#include "ipc.h"  // For ipc_handle_t and related function pointers

/* Mocking system calls */
static int mock_socket(int domain, int type, int protocol) {
    check_expected(domain);
    check_expected(type);
    check_expected(protocol);
    return mock_type(int);
}

static int mock_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    check_expected(sockfd);
    check_expected(addr);
    check_expected(addrlen);
    return mock_type(int);
}

static int mock_listen(int sockfd, int backlog) {
    check_expected(sockfd);
    check_expected(backlog);
    return mock_type(int);
}

static int mock_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    check_expected(sockfd);
    check_expected(addr);
    check_expected(addrlen);
    return mock_type(int);
}

static ssize_t mock_send(int sockfd, const void *buf, size_t len, int flags) {
    check_expected(sockfd);
    check_expected(buf);
    check_expected(len);
    check_expected(flags);
    return mock_type(ssize_t);
}

static ssize_t mock_recv(int sockfd, void *buf, size_t len, int flags) {
    check_expected(sockfd);
    check_expected(buf);
    check_expected(len);
    check_expected(flags);
    return mock_type(ssize_t);
}

static int mock_close(int fd) {
    check_expected(fd);
    return mock_type(int);
}

static int mock_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    check_expected(sockfd);
    check_expected(addr);
    check_expected(addrlen);
    return mock_type(int);
}

static void *mock_malloc(size_t size) {
    check_expected(size);
    return mock_type(void *);
}

/* Test IPC Socket Init (Server) */
static void test_ipc_socket_init_server(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 1); // Server mode

    expect_value(mock_socket, domain, AF_INET);
    expect_value(mock_socket, type, SOCK_STREAM);
    expect_value(mock_socket, protocol, 0);
    will_return(mock_socket, 3); // Assume the socket returns FD 3

    expect_value(mock_bind, sockfd, 3);
    expect_any(mock_bind, addr);  // Match any address
    expect_value(mock_bind, addrlen, sizeof(struct sockaddr_in));
    will_return(mock_bind, 0);

    expect_value(mock_listen, sockfd, 3);
    expect_value(mock_listen, backlog, 1);
    will_return(mock_listen, 0);

    int result = sock->init(sock); // Using the function pointer
    assert_int_equal(result, IPC_SUCCESS);

    sock->destroy(sock); // Cleanup
}

/* Test IPC Socket Init (Client) */
static void test_ipc_socket_init_client(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 0); // Client mode

    expect_value(mock_socket, domain, AF_INET);
    expect_value(mock_socket, type, SOCK_STREAM);
    expect_value(mock_socket, protocol, 0);
    will_return(mock_socket, 3); // Assume socket returns FD 3

    expect_value(mock_connect, sockfd, 3);
    expect_any(mock_connect, addr);  // Match any address
    expect_value(mock_connect, addrlen, sizeof(struct sockaddr_in));
    will_return(mock_connect, 0);

    int result = sock->init(sock); // Using the function pointer
    assert_int_equal(result, IPC_SUCCESS);

    sock->destroy(sock); // Cleanup
}

/* Test IPC Socket Send */
static void test_ipc_socket_send(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 0); // Client mode

    expect_value(mock_send, sockfd, 3);
    expect_string(mock_send, buf, "test message");
    expect_value(mock_send, len, strlen("test message"));
    expect_value(mock_send, flags, 0);
    will_return(mock_send, strlen("test message"));

    int result = sock->send(sock, "test message", strlen("test message")); // Using function pointer
    assert_int_equal(result, IPC_SUCCESS);

    sock->destroy(sock); // Cleanup
}

/* Test IPC Socket Receive */
static void test_ipc_socket_receive(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 0); // Client mode
    char buffer[1024];

    expect_value(mock_recv, sockfd, 3);
    expect_any(mock_recv, buf);
    expect_value(mock_recv, len, sizeof(buffer));
    expect_value(mock_recv, flags, 0);
    will_return(mock_recv, 10);  // Assume 10 bytes received

    int result = sock->receive(sock, buffer, sizeof(buffer)); // Using function pointer
    assert_int_equal(result, IPC_SUCCESS);

    sock->destroy(sock); // Cleanup
}

/* Test IPC Socket Destroy */
static void test_ipc_socket_destroy(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 0); // Client mode

    expect_value(mock_close, fd, 3);
    will_return(mock_close, 0);

    int result = sock->destroy(sock); // Using function pointer
    assert_int_equal(result, IPC_SUCCESS);
}

/* Test IPC Socket Accept */
static void test_ipc_socket_accept(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *server_sock = ipc_socket_create("127.0.0.1", 8080, 1); // Server mode
    ipc_handle_t *client_sock;

    expect_value(mock_accept, sockfd, 3);
    expect_any(mock_accept, addr);  // Match any address
    expect_any(mock_accept, addrlen); // Match any address length
    will_return(mock_accept, 4);  // Simulate successful accept returning FD 4

    client_sock = server_sock->accept(server_sock); // Using function pointer
    assert_non_null(client_sock);

    server_sock->destroy(server_sock); // Cleanup server
    client_sock->destroy(client_sock); // Cleanup client
}

/* Test IPC Socket Create */
static void test_ipc_socket_create(void **state) {
    (void) state; // Unused variable

    ipc_handle_t *sock = ipc_socket_create("127.0.0.1", 8080, 1); // Server mode

    assert_non_null(sock);
    assert_int_equal(((ipc_socket_t *)sock)->sockfd, 3); // Assume socket creation succeeds

    sock->destroy(sock); // Cleanup
}

/* Main function for running the tests */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ipc_socket_init_server),
        cmocka_unit_test(test_ipc_socket_init_client),
        cmocka_unit_test(test_ipc_socket_send),
        cmocka_unit_test(test_ipc_socket_receive),
        cmocka_unit_test(test_ipc_socket_destroy),
        cmocka_unit_test(test_ipc_socket_accept),
        cmocka_unit_test(test_ipc_socket_create),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

