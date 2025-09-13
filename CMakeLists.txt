# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.22)

set(QDASH_NAME QDash)
set(QDASH_ORG "QFRC")
set(QDASH_REPO https://git.crueter.xyz/${QDASH_ORG}/${QDASH_NAME})

project(${QDASH_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTORCC ON)

set(BUILD_SHARED_LIBS OFF)

include(CMakeDependentOption)

option(QDASH_WEBVIEW OFF "Enable web view widget (Requires QtWebEngine)")
option(QDASH_CAMVIEW ON "Enable camera view widget (Requires QtMultimedia)")
cmake_dependent_option(QDASH_NETWORK "Enable remote layout feature (Requires QtNetwork)" ON "NOT QDASH_WEBVIEW" ON)
option(USE_CCACHE OFF "Change some build settings to improve ccache reliability.")

set(QDASH_QT_COMPONENTS Core Quick QuickControls2)
set(QDASH_QT_LIBRARIES Qt6::Core Qt6::Quick Qt6::QuickControls2)

if (QDASH_WEBVIEW)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} WebEngineQuick)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::WebEngineQuick)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_WEBVIEW)
endif()

if (QDASH_CAMVIEW)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} Multimedia)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::Multimedia)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_CAMVIEW)
endif()

if (QDASH_NETWORK)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} Network)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::Network)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_NETWORK)
endif()

if (USE_CCACHE AND SIN32)
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()
find_package(Qt6 6.7 REQUIRED COMPONENTS ${QDASH_QT_COMPONENTS})

qt_standard_project_setup(REQUIRES 6.7)

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/dist/${QDASH_NAME}.rc)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Modules
include(GetSCMRev)
include(GetBuildPlatform)

# Build Date
string(TIMESTAMP TODAY "%Y-%m-%d")
set(QDASH_BUILD_TIMESTAMP "${TODAY}")

add_subdirectory(dist)
add_subdirectory(ntcore EXCLUDE_FROM_ALL)
find_package(ntcore)

set(QT_QML_GENERATE_QMLLS_INI ON)

add_subdirectory(QDash)
