cmake_minimum_required(VERSION 3.8)
project(net)

# set(CMAKE_CXX_STANDARD 11) -std=c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -Wall -ggdb3 -pthread")
set(SOCKET_LIB
        common/socket.cpp)

set(LIBTPEVENT_PATH libtpevent/src)
set(LIBTPEVENT
        ${LIBTPEVENT_PATH}/options.cpp
        ${LIBTPEVENT_PATH}/utils.cpp
        ${LIBTPEVENT_PATH}/server.cpp
        ${LIBTPEVENT_PATH}/engine.cpp
        ${LIBTPEVENT_PATH}/select.cpp
        ${LIBTPEVENT_PATH}/poll.cpp
        ${LIBTPEVENT_PATH}/poll_async.cpp)

add_library(socket_lib STATIC ${SOCKET_LIB})
add_library(libtpevent_lib STATIC ${LIBTPEVENT})

add_executable(prefork prefork/prefork.cpp)
add_executable(multiplex multiplexing/multiplex.cpp)
add_executable(client_read_timeout client_read_timeout/client_timeouts.cpp)
add_executable(client_connect_timeout client_connect_timeout/client_timeouts.cpp)
add_executable(simple_server simple_server/simple_server.cpp)
add_executable(lib_event_main libevent/main.cpp)
add_executable(http_client http_client/http_client.cpp)
add_executable(asio asio/main.cpp)
add_executable(libtpevent "${LIBTPEVENT_PATH}/main.cpp")

target_link_libraries(client_read_timeout socket_lib)
target_link_libraries(libtpevent libtpevent_lib -lboost_system)
target_link_libraries(prefork socket_lib)
target_link_libraries(client_connect_timeout socket_lib)
target_link_libraries(simple_server socket_lib)
target_link_libraries(lib_event_main socket_lib "-levent")
target_link_libraries(http_client socket_lib)
target_link_libraries(multiplex socket_lib)
target_link_libraries(asio socket_lib -lboost_system -lpthread -std=c++14)

add_executable(simpliest_client simplest_client_server/client.cpp)
add_executable(server_select simplest_client_server/server_select.cpp)
add_executable(server_fork simplest_client_server/server_fork.cpp)
add_executable(server_poll simplest_client_server/server_poll.cpp)