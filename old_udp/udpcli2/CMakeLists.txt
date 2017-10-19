cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
project(udpcli2)

set(SRCS
        client.cpp
        )

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
