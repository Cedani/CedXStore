list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_PREFIX_PATH}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(SRC
    main.cpp
)

qt_add_executable(Launcher ${SRC})
target_link_libraries(Launcher PRIVATE Qt6::Core Qt6::Widgets)
set_target_properties(Launcher PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/"
    WIN32_EXECUTABLE ON
)

