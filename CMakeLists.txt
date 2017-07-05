cmake_minimum_required(VERSION 3.0)
project (udp-fun)

set (CMAKE_CXX_STANDARD 14)

# Threads
find_package(Threads)

#Boost
find_package(Boost COMPONENTS system REQUIRED)

# WinSock
if(WIN32)
set(WINSOCK ws2_32)
endif()

include_directories(${Boost_INCLUDE_DIRS})

set(ALL_LIBS ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${WINSOCK})

# Build all examples
add_subdirectory(cli1)
add_subdirectory(cli2)
add_subdirectory(serv1)
add_subdirectory(serv2)
