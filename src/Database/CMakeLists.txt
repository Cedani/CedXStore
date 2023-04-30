message(${SQL_PATH})
add_library(LibDatabase SHARED MySqlDb.cpp)
# add_library(LibDatabase SHARED test.cpp)


if (UNIX)
    target_link_libraries(LibDatabase PUBLIC unofficial::mysql-connector-cpp::connector nlohmann_json::nlohmann_json resolv)
else()
    target_link_libraries(LibDatabase PRIVATE mysqlcppconn nlohmann_json::nlohmann_json)
endif()
target_include_directories(LibDatabase PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(LibDatabase PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)