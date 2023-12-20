find_package(OpenSSL REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SRC_SERVER
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Core.cpp
)

add_executable(LauncherServer ${SRC_SERVER})
target_link_libraries(LauncherServer PRIVATE LibDatabase LibTcpServer nlohmann_json::nlohmann_json OpenSSL::Crypto OpenSSL::SSL cryptopp::cryptopp)
set_target_properties(LauncherServer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)
