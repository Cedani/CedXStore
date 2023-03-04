cmake_minimum_required(VERSION 3.20)

project(Arcade)

set(AUTHOR "Cedani")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread")
endif()

include(C:/Users/Cedani/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(LIB_DTB_ENV_PATH "${CMAKE_SOURCE_DIR}/utils/env")


configure_file(${CMAKE_SOURCE_DIR}/src/Config.h.in ${CMAKE_SOURCE_DIR}/src/Config.h)


include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
# find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Threads)



# find_package(Threads CONFIG REQUIRED)
# find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
# include_directories(${CONAN_INCLUDE_DIRS})

set(PROJECT_SRC 
    src/main.cpp
)

add_subdirectory(src/Threadpool)
add_subdirectory(src/Database)
add_subdirectory(src/TCP/server)


add_executable(Arcade ${PROJECT_SRC})
target_link_libraries(Arcade PRIVATE LibDatabase LibTcpServer LibThread ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(Arcade PRIVATE LibDatabase ${MYSQLPP32})

