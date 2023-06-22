cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME RussianCheckers)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include_directories(${INCLUDE_DIR})

set(COMMON_FILES
    ${SRC_DIR}/checkerslogic.cpp
    ${INCLUDE_DIR}/checkerslogic.h
    ${SRC_DIR}/direction.cpp
    ${INCLUDE_DIR}/direction.h
    ${SRC_DIR}/constants.cpp
    ${INCLUDE_DIR}/constants.h
    ${INCLUDE_DIR}/defines.h
    ${SRC_DIR}/bit_algo.cpp
    ${INCLUDE_DIR}/bit_algo.h
    ${SRC_DIR}/boardstate.cpp
    ${INCLUDE_DIR}/boardstate.h
)

set(APP_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/model.cpp
    ${INCLUDE_DIR}/model.h
    ${SRC_DIR}/history.cpp
    ${INCLUDE_DIR}/history.h
)

add_library(checkersLibrary ${COMMON_FILES})

qt_add_executable(appRussianCheckers ${APP_FILES})
qt_add_qml_module(appRussianCheckers
    URI RussianCheckers
    VERSION 1.0
    QML_FILES main.qml
    NO_RESOURCE_TARGET_PATH
)

set_target_properties(appRussianCheckers PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER checkers_gui
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRussianCheckers
    PRIVATE Qt6::Quick
    PUBLIC checkersLibrary
)

install(TARGETS appRussianCheckers
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(./googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests
    ${TEST_DIR}/test_logic.cpp
    ${TEST_DIR}/test_bit_algo.cpp
    ${TEST_DIR}/test_direction.cpp
    ${TEST_DIR}/test_board_state.cpp
)
target_link_libraries(runUnitTests gtest gtest_main checkersLibrary)

add_test(UnitTests runUnitTests)
