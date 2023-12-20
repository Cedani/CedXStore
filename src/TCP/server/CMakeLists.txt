find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost COMPONENTS thread system REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Connection
)
    
set(LIB_SERVER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Connection/Connection.cpp
    Server.cpp
)

add_library(LibTcpServer SHARED ${LIB_SERVER_SRC})
target_link_libraries(LibTcpServer PUBLIC nlohmann_json::nlohmann_json LibThread Boost::system Boost::thread)
target_include_directories(LibTcpServer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Connection
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(LibTcpServer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)