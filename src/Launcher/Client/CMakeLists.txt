list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_PREFIX_PATH}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Core5Compat)
qt_standard_project_setup()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Login
    ${CMAKE_CURRENT_SOURCE_DIR}/Login/ClickableLabel
)

set(SRC
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Login/Login.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Login/ClickableLabel/ClickableLabel.cpp
)

qt_add_executable(Launcher ${SRC})
target_link_libraries(Launcher PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Core5Compat)
set_target_properties(Launcher PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    WIN32_EXECUTABLE ON
)

