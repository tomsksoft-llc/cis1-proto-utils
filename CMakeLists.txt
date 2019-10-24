cmake_minimum_required(VERSION 3.9)

project(cis1_proto_utils)

set(${PROJECT_NAME}_SRC
    src/transaction.cpp
    src/cloexec.cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
