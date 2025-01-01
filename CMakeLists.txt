cmake_minimum_required(VERSION 3.5)

project(project_SDE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)

include_directories(${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS})
include_directories(${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS})
include_directories(${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        BackupManager.cpp
        BackupManager.h
        RestoreManager.cpp
        RestoreManager.h
        BackupWindow.cpp
        BackupWindow.h
        backupwindow.ui
        RestoreWindow.cpp
        RestoreWindow.h
        restorewindow.ui
)

message(STATUS "Project sources: ${PROJECT_SOURCES}")

qt_add_executable(project_SDE
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(project_SDE PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

set_target_properties(project_SDE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(project_SDE)

