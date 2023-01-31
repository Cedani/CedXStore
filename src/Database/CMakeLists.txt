add_library(LibDatabase SHARED MySqlDb.cpp)
target_link_libraries(LibDatabase PUBLIC unofficial::mysql-connector-cpp::connector nlohmann_json::nlohmann_json)
target_include_directories(LibDatabase PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(LibDatabase PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/"
)