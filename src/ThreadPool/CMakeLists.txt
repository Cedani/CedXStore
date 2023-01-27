find_package(Threads)
add_library(LibThread SHARED ThreadPool.cpp)
target_link_libraries(LibThread ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(LibThread PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")