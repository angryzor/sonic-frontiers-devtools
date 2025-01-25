cmake_minimum_required(VERSION 3.28)
project(uwebsockets LANGUAGES CXX C)

set(LIBUV_BUILD_SHARED OFF)
add_subdirectory(libuv)

add_library(usockets STATIC)
target_link_libraries(usockets PRIVATE uv_a)
target_compile_features(usockets PRIVATE cxx_std_20)
target_compile_definitions(usockets PRIVATE LIBUS_USE_LIBUV=1 LIBUS_NO_SSL=1)
target_sources(usockets
    PRIVATE
        uWebSockets/uSockets/src/crypto/openssl.c
        uWebSockets/uSockets/src/eventing/epoll_kqueue.c
        uWebSockets/uSockets/src/eventing/gcd.c
        uWebSockets/uSockets/src/eventing/libuv.c
        uWebSockets/uSockets/src/io_uring/io_context.c
        uWebSockets/uSockets/src/io_uring/io_loop.c
        uWebSockets/uSockets/src/io_uring/io_socket.c
        uWebSockets/uSockets/src/bsd.c
        uWebSockets/uSockets/src/context.c
        uWebSockets/uSockets/src/loop.c
        uWebSockets/uSockets/src/socket.c
        uWebSockets/uSockets/src/udp.c
    PUBLIC FILE_SET HEADERS BASE_DIRS uWebSockets/uSockets/src FILES
        uWebSockets/uSockets/src/libusockets.h
)

add_library(uwebsockets INTERFACE)
target_link_libraries(uwebsockets INTERFACE usockets)
target_compile_features(uwebsockets INTERFACE cxx_std_20)
target_compile_definitions(uwebsockets INTERFACE UWS_NO_ZLIB=1)
target_sources(uwebsockets
    PUBLIC FILE_SET HEADERS BASE_DIRS uWebSockets/src FILES
        uWebSockets/src/App.h
)
