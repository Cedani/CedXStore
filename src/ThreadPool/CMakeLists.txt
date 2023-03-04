add_library(LibThread SHARED ThreadPool.cpp)
target_link_libraries(LibThread ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(LibThread PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(LibThread PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/"
)