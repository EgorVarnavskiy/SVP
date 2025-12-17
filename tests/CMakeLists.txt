set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(test_gamelogic
    test_gamelogic.cpp
    ../src/gamelogic.cpp
)

target_include_directories(test_gamelogic PRIVATE ${INCLUDE_DIRS})
target_link_libraries(test_gamelogic
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
)

add_executable(test_gameboard
    test_gameboard.cpp
    ../src/gameboard.cpp
    ../src/gamelogic.cpp
)

target_include_directories(test_gameboard PRIVATE ${INCLUDE_DIRS})
target_link_libraries(test_gameboard
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

if(Qt5_FOUND)
    add_test(NAME test_gamelogic COMMAND test_gamelogic)
    add_test(NAME test_gameboard COMMAND test_gameboard)
endif()

if(WIN32 AND Qt5_FOUND)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QT_DLL_DIR ${QtCore_location} DIRECTORY)

    get_target_property(QtGui_location Qt5::Gui LOCATION)
    get_filename_component(QT_PLUGINS_DIR ${QtGui_location} DIRECTORY)
    set(QT_PLUGINS_DIR "${QT_PLUGINS_DIR}/../plugins")

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

    add_custom_command(TARGET test_gameboard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:test_gameboard>/platforms"
    )

    add_custom_command(TARGET test_gameboard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:test_gameboard>/platforms/"
    )
endif()

target_compile_options(test_gamelogic PRIVATE -w)
target_compile_options(test_gameboard PRIVATE -w)
