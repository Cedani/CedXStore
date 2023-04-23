cmake_minimum_required(VERSION 3.20)

project(Arcade)

set(AUTHOR "Cedani")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++20 -g -pthread")
endif()


include($ENV{VcpkgPath})
# if (WIN32)
#     include(C:/Users/Cedani/vcpkg/triplets/x64-windows.cmake)
# endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(LIB_DTB_ENV_PATH "${CMAKE_SOURCE_DIR}/utils/env")
set(RESOURCES "${CMAKE_SOURCE_DIR}/Resources/")


configure_file(${CMAKE_SOURCE_DIR}/src/Config.h.in ${CMAKE_SOURCE_DIR}/src/Config.h)


include_directories(${CMAKE_SOURCE_DIR}/src)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
# find_package(gtkmm CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

# pkg_check_modules(GTK4 REQUIRED gtk4)
# PKG_CHECK_MODULES(GTKMM REQUIRED "C:/Users/Cedani/vcpkg/installed/x86-windows/lib/pkgconfig/gtkmm-4.0.pc")
# find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Threads)



# find_package(Threads CONFIG REQUIRED)
# find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
# include_directories(${CONAN_INCLUDE_DIRS})

set(PROJECT_SRC 
src/main.cpp
)

add_subdirectory(src/ThreadPool)
add_subdirectory(src/Database)
add_subdirectory(src/TCP/server)
add_subdirectory(src/Launcher)


add_executable(Arcade ${PROJECT_SRC})
target_link_libraries(Arcade PRIVATE LibDatabase LibTcpServer LibThread ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(Arcade PRIVATE LibDatabase ${MYSQLPP32})

