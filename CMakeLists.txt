cmake_minimum_required(VERSION 3.16)
project(TicTacToe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(TARGET_NAME TicTacToe)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/gameboard.cpp
    src/gamelogic.cpp
)

set(HEADERS
    src/mainwindow.h
    src/gameboard.h
    src/gamelogic.h
)

set(FORMS
    src/mainwindow.ui
)

add_executable(${TARGET_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
)

target_link_libraries(${TARGET_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)

set_target_properties(${TARGET_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_include_directories(${TARGET_NAME} PRIVATE src)

if(WIN32 AND Qt5_FOUND)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QT_DLL_DIR ${QtCore_location} DIRECTORY)

    get_target_property(QtGui_location Qt5::Gui LOCATION)
    get_filename_component(QT_PLUGINS_DIR ${QtGui_location} DIRECTORY)
    set(QT_PLUGINS_DIR "${QT_PLUGINS_DIR}/../plugins")

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_DLL_DIR}/Qt5Core.dll"
            "${QT_DLL_DIR}/Qt5Widgets.dll"
            "${QT_DLL_DIR}/Qt5Gui.dll"
            $<TARGET_FILE_DIR:${TARGET_NAME}>
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/platforms"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/platforms/"
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    enable_testing()
    add_subdirectory(tests)
endif()
