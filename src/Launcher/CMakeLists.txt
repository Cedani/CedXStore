# link_directories(${GTKMM_LIBRARY_DIRS})
# include_directories(${GTKMM_INCLUDE_DIRS})
# include(${wxWidgets_USE_FILE})


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/Client
    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/Window
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Manager/Manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/App/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/myWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(Launcher ${SRC})
target_link_libraries(Launcher wx::core wx::base)

set_target_properties(Launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/"
)