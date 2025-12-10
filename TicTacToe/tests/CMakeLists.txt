cmake_minimum_required(VERSION 3.16)

project(TicTacToeTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

include(GoogleTest)

set(gtest_disable_patching ON)
set(gtest_force_shared_crt ON)
set(test_gamelogic_SOURCES
    sources/test_gamelogic.cpp
    ../src/sources/gamelogic.cpp
)

qt5_wrap_cpp(test_gamelogic_MOC_SOURCES
    ../src/headers/gamelogic.h
)

add_executable(test_gamelogic
    ${test_gamelogic_SOURCES}
    ${test_gamelogic_MOC_SOURCES}
)

target_include_directories(test_gamelogic
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/headers
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(test_gamelogic
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
)

set(test_gameboard_SOURCES
    sources/test_gameboard.cpp
    ../src/sources/gameboard.cpp
    ../src/sources/gamelogic.cpp
)

qt5_wrap_cpp(test_gameboard_MOC_SOURCES
    ../src/headers/gamelogic.h
    ../src/headers/gameboard.h
)

add_executable(test_gameboard
    ${test_gameboard_SOURCES}
    ${test_gameboard_MOC_SOURCES}
)

target_include_directories(test_gameboard
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/headers
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(test_gameboard
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)

if(WIN32)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QT_DLL_DIR ${QtCore_location} DIRECTORY)

    add_custom_command(TARGET test_gamelogic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_DLL_DIR}/Qt5Core.dll"
            "${QT_DLL_DIR}/Qt5Test.dll"
            $<TARGET_FILE_DIR:test_gamelogic>
    )

    add_custom_command(TARGET test_gameboard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_DLL_DIR}/Qt5Core.dll"
            "${QT_DLL_DIR}/Qt5Test.dll"
            "${QT_DLL_DIR}/Qt5Widgets.dll"
            "${QT_DLL_DIR}/Qt5Gui.dll"
            $<TARGET_FILE_DIR:test_gameboard>
    )
endif()

target_compile_options(test_gamelogic PRIVATE -w)
target_compile_options(test_gameboard PRIVATE -w)

option(GTEST_DISCOVER_TESTS "Use gtest_discover_tests" OFF)

if(GTEST_DISCOVER_TESTS)
    gtest_discover_tests(test_gamelogic)
    gtest_discover_tests(test_gameboard)
else()
    add_test(NAME test_gamelogic COMMAND test_gamelogic)
    add_test(NAME test_gameboard COMMAND test_gameboard)
endif()

