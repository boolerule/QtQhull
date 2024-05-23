cmake_minimum_required(VERSION 3.14)

project(QtQhull LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core gui LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core gui LinguistTools)
# 导入qhull库
find_package(qhull REQUIRED)

set(TS_FILES QtQhull_zh_CN.ts)

add_library(QtQhull STATIC
  QtQhull_global.h
  qtqhull.cpp
  qtqhull.h
  ${TS_FILES}
)

target_link_libraries(QtQhull PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                        Qt${QT_VERSION_MAJOR}::Gui
                                        ${libqhull_LIBRARY}
                                        ${libqhull_static_r_LIBRARY}
)

target_compile_definitions(QtQhull PRIVATE QTQHULL_LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
