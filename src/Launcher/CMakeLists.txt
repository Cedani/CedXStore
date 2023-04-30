# link_directories(${GTKMM_LIBRARY_DIRS})
# include_directories(${GTKMM_INCLUDE_DIRS})
# include(${wxWidgets_USE_FILE})
# set(wxBUILD_SHARED ON)
# include(${wxWidgets_USE_FILE})
message("test here")
# message("${wxWidgets_USE_FILE}")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/App
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Window
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Window/SideMenu
    ${CMAKE_CURRENT_SOURCE_DIR}/Server/Core/
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client/Client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Manager/Manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/App/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Window/myWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Window/myButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Window/SideMenu/SideMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/main.cpp
)

set(SRC_SERVER
    ${CMAKE_CURRENT_SOURCE_DIR}/Server/main.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Server/Core/Core.cpp
)

add_executable(Launcher ${SRC})
target_link_libraries(Launcher PRIVATE wx::core wx::base)
set_target_properties(Launcher PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)

add_executable(LauncherServer ${SRC_SERVER})
target_link_libraries(LauncherServer PRIVATE LibDatabase nlohmann_json::nlohmann_json Boost::headers Threads::Threads OpenSSL::Crypto OpenSSL::SSL)
set_target_properties(LauncherServer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
)