# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(QRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/fields.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/accents.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc)

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

add_subdirectory(Config)
add_subdirectory(Dialogs)
add_subdirectory(Fields)
add_subdirectory(Items)
add_subdirectory(Widgets)
add_subdirectory(Main)

set(BASE_PLUGINS
    QDashQmlConfig
    QDashQmlDialogs
    QDashQmlFields
    QDashQmlItems
    QDashQmlMain)

foreach(PLUGIN ${BASE_PLUGINS})
    set(QML_PLUGINS ${QML_PLUGINS} ${PLUGIN}plugin ${PLUGIN})
endforeach()

qt_add_library(QDashQml STATIC)

target_link_libraries(QDashQml
    PRIVATE
    BuildConfig

    ${QDASH_QT_LIBRARIES}
    ${QML_PLUGINS}
    QDashQmlWidgets)

add_subdirectory(Native)
