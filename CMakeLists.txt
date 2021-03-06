cmake_minimum_required(VERSION 3.9)

project(cis1_proto_utils)

option(BUILD_TESTING "Build tests" OFF)

set(${PROJECT_NAME}_SRC
    src/transaction.cpp
    src/cloexec.cpp
    src/param_codec.cpp
    src/read_istream_kv_str.cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${CONAN_LIBS})

if(BUILD_TESTING)
    add_subdirectory(test_package)
endif(BUILD_TESTING)
