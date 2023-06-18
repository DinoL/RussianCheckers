cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME RussianCheckers)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
include_directories(${INCLUDE_DIR})

set(SRC_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/model.cpp
    ${INCLUDE_DIR}/model.h
)

qt_add_executable(appRussianCheckers ${SRC_FILES})
qt_add_qml_module(appRussianCheckers
    URI RussianCheckers
    VERSION 1.0
    QML_FILES main.qml
    RESOURCE_PREFIX ":qt/qml/"
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
)

install(TARGETS appRussianCheckers
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
