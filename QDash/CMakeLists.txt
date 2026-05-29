# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(QRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/fields.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/icons.qrc)

# Android and iOS can only use a single file.
# TODO(crueter): WASM has to store stuff... somehow
if (ANDROID OR IOS)
    set(QDASH_SINGLEMODE ON)
    add_compile_definitions(QDASH_SINGLEMODE)
endif()

# Fix QML imports
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml" CACHE PATH "Install path for QML" FORCE)
set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/qml CACHE STRING "Extra QML Import Paths" FORCE)

# Put binaries into <build>/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(Controls)
add_subdirectory(Dialogs)
add_subdirectory(Core)
add_subdirectory(Components)
add_subdirectory(Widgets)

set(BASE_PLUGINS
    QDashQmlControls
    QDashQmlDialogs
    QDashQmlComponents
    QDashQmlCore
    QDashQmlWidgets)

foreach(PLUGIN ${BASE_PLUGINS})
    set(QML_PLUGINS ${QML_PLUGINS} ${PLUGIN}plugin ${PLUGIN})
endforeach()

qt_add_library(QDashQml STATIC)

target_link_libraries(QDashQml
    PRIVATE
        BuildConfig
        ${QML_PLUGINS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Backend")
include_directories("$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
add_compile_definitions("${QDASH_DEFINES}")

add_subdirectory(Backend)

# Main executable
add_subdirectory(Main)
