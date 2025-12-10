set(SOURCES
    sources/main.cpp
    sources/mainwindow.cpp
    sources/gameboard.cpp
    sources/gamelogic.cpp
)

set(HEADERS
    headers/mainwindow.h
    headers/gameboard.h
    headers/gamelogic.h
)

set(UI_FILES
    ui/mainwindow.ui
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(TicTacToe
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
else()
    add_executable(TicTacToe
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
endif()

target_include_directories(TicTacToe
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(TicTacToe
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(TicTacToe Qt6::Core5Compat)
endif()
