add_library(LibThread SHARED ThreadPool.cpp)
target_link_libraries(LibThread ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(LibThread PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(LibThread PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)