cmake_minimum_required(VERSION 3.9)

project(tests)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(BUILD_TESTING)
include_directories(${CMAKE_SOURCE_DIR}/test_package/include)
else()
include_directories(${CMAKE_SOURCE_DIR}/include)
endif()

add_executable(
    tests
    src/read_istream_kv_str.cpp
    src/param_codec.cpp)

set_property(TARGET tests PROPERTY CXX_STANDARD 17)

target_link_libraries(tests cis1_proto_utils ${CONAN_LIBS})

enable_testing()
add_test(NAME tests
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND ${PROJECT_NAME})
