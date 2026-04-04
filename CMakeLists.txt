cmake_minimum_required(VERSION 3.10)
project(TrayApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} WIN32 
    src/main.cpp
    src/MainWindow.cpp
    src/TrayIcon.cpp
    src/TrayApp_.cpp
)

target_link_libraries(${PROJECT_NAME}
    user32
    shell32
    comctl32
    gdi32
)

set(RESOURCE_FILES src/TrayApp_.rc)
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    _UNICODE
    UNICODE
    _CRT_SECURE_NO_WARNINGS
)
