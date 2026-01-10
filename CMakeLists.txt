# SPDX-FileCopyrightText: Copyright 2026 crueter
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

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")

## Options ##

option(QDASH_STATIC OFF "Build QDash as a static executable.")
option(QDASH_BUNDLE_QT OFF "Use a bundled Qt build")
option(QDASH_WEBVIEW OFF "Enable web view widget (Requires QtWebEngine)")
option(QDASH_CAMVIEW ON "Enable camera view widget (Requires QtMultimedia)")
cmake_dependent_option(QDASH_NETWORK "Enable remote layout feature (Requires QtNetwork)" ON "NOT QDASH_WEBVIEW" ON)

set(QT_NO_PRIVATE_MODULE_WARNING ON)
set(QDASH_QT_COMPONENTS Core Quick QuickControls2 Widgets)
set(QDASH_QT_OPTIONAL GuiPrivate QuickTemplates2Private QuickPrivate)
set(QDASH_QT_LIBRARIES Qt6::Core Qt6::Quick Qt6::QuickControls2 Qt6::Widgets Carboxyl::Carboxyl)

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

## Modules ##
include(DefaultConfig)
include(DetectArchitecture)
include(DetectPlatform)
include(UseCcache)
include(UseLTO)
include(FasterLinker)
include(GetSCMRev)
include(CPMUtil)
include(QDashModule)

if (QDASH_BUNDLE_QT)
    AddQt(6.9.3)
    if (APPLE OR NOT UNIX)
        set(QDASH_STATIC ON)
    endif()
endif()

if (QDASH_STATIC)
    include(StaticBuild)

    # static builds require explicitly provided zlib
    AddJsonPackage(zlib)
endif()

find_package(Qt6 6.4 REQUIRED
    COMPONENTS ${QDASH_QT_COMPONENTS}
    OPTIONAL_COMPONENTS ${QDASH_QT_OPTIONAL})

if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()

qt_standard_project_setup(REQUIRES 6.4)

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/dist/${QDASH_NAME}.rc)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Build Date
string(TIMESTAMP TODAY "%Y-%m-%d")
set(QDASH_BUILD_TIMESTAMP "${TODAY}")

add_subdirectory(dist)
add_subdirectory(externals)

find_package(ntcore)
find_package(Carboxyl)

set(QT_QML_GENERATE_QMLLS_INI ON)

add_subdirectory(QDash)
