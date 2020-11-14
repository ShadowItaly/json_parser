cmake_minimum_required(VERSION 2.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(json_parser)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.
conan_basic_setup()  # Prepares the CMakeList.txt for Conan.
find_package(Git)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 17)
add_executable(tests.o tests/main.cpp)

target_link_libraries(tests.o ${CONAN_LIBS})
