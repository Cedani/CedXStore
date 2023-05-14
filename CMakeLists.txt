cmake_minimum_required(VERSION 3.20)

project(Arcade)

set(AUTHOR "Cedani")

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++20 -g3")
endif()


include($ENV{VcpkgPath})
# if (WIN32)
#     include(C:/Users/Cedani/vcpkg/triplets/x64-windows.cmake)
# endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(LIB_DTB_ENV_PATH "${CMAKE_SOURCE_DIR}/utils/env")
set(TEMP_ENDPOINT "${CMAKE_SOURCE_DIR}/utils/endpoint")
set(RESOURCES "${CMAKE_SOURCE_DIR}/Resources/")
set(SQL_PATH $ENV{MySqlPath})
include_directories(
    ${SQL_PATH}/include/
)
link_directories($ENV{MySqlPath}/lib/vs14)

configure_file(${CMAKE_SOURCE_DIR}/src/Config.h.in ${CMAKE_SOURCE_DIR}/src/Config.h)


include_directories(${CMAKE_SOURCE_DIR}/src)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


find_package(nlohmann_json CONFIG REQUIRED)
# find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
# find_package(gtkmm CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

# pkg_check_modules(GTK4 REQUIRED gtk4)
# PKG_CHECK_MODULES(GTKMM REQUIRED "C:/Users/Cedani/vcpkg/installed/x86-windows/lib/pkgconfig/gtkmm-4.0.pc")
# find_package(Boost COMPONENTS thread system REQUIRED)
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/external/Boost")
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
    # set(Boost_COMPILER "-vc143")
    set(BOOST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/Boost/include/boost-1_82/boost")
    set(BOOST_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/external/Boost/lib")
    # find_package(Threads REQUIRED)
elseif(UNIX)
    # set(Boost_COMPILER "-vc143")
    set(BOOST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/Boost/include/boost")
    set(BOOST_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/external/Boost/lib")
endif()


find_package(Threads)
find_package(cryptopp CONFIG REQUIRED)
find_package(Boost COMPONENTS thread system headers REQUIRED)
find_package(OpenSSL REQUIRED)

# Boost
# find_package(Boost REQUIRED COMPONENTS headers )



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

