find_package(Threads)
find_package(cryptopp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)


add_library(LibDatabase SHARED Database.cpp)
target_link_libraries(LibDatabase PRIVATE nlohmann_json::nlohmann_json Threads::Threads OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(LibDatabase PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(LibDatabase PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)