cmake_minimum_required(VERSION 3.10)

project(Arcade)

set(AUTHOR "Cedani")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# if(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread")
# endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_DISABLE_CHECK_COMPILER True CACHE BOOL "" FORCE)
conan_basic_setup()
set(LIB_THREAD_PATH "${CMAKE_BINARY_DIR}/build/bin/LibThread.dll")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
configure_file(${CMAKE_SOURCE_DIR}/src/Config.h.in ${CMAKE_SOURCE_DIR}/src/Config.h)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(Threads)
include_directories(${CONAN_INCLUDE_DIRS})

set(PROJECT_SRC 
    src/main.cpp
)

add_subdirectory(src/Threadpool)


add_executable(Arcade ${PROJECT_SRC})
target_link_libraries(Arcade PUBLIC ${CONAN_LIBS} LibThread ${CMAKE_THREAD_LIBS_INIT})

