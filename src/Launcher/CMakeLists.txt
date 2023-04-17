# link_directories(${GTKMM_LIBRARY_DIRS})
# include_directories(${GTKMM_INCLUDE_DIRS})
# include(${wxWidgets_USE_FILE})
# set(wxBUILD_SHARED ON)
# include(${wxWidgets_USE_FILE})
message("test here")
# message("${wxWidgets_USE_FILE}")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/Client
    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/Window
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/SideMenu
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Manager/Manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/App/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/myWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/myButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/SideMenu/SideMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(Launcher ${SRC})
target_link_libraries(Launcher wx::core wx::base)
set_target_properties(Launcher PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)