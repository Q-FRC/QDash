# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.31)

set(QDASH_NAME QDash)
set(QDASH_ORG "QFRC")
set(QDASH_REPO https://git.crueter.xyz/${QDASH_ORG}/${QDASH_NAME})

project(${QDASH_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)

include(CMakeDependentOption)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")

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

## Options ##

option(QDASH_STATIC "Build QDash as a static executable" OFF)
option(QDASH_BUNDLE_QT "Use a bundled Qt build" OFF)
option(QDASH_WEBVIEW "Enable web view widget (Requires QtWebEngine)" OFF)
option(QDASH_CAMVIEW "Enable camera view widget (Requires QtMultimedia)" ON)
cmake_dependent_option(QDASH_NETWORK "Enable remote layout feature (Requires QtNetwork)" ON "NOT QDASH_WEBVIEW" ON)

set(QT_NO_PRIVATE_MODULE_WARNING ON)
set(QDASH_QT_COMPONENTS Core Quick QuickControls2 QmlCore  QuickDialogs2
    QuickLayouts QuickEffects QuickControls2Basic QuickControls2Material QuickControls2Fusion QuickControls2Universal)
set(QDASH_QT_OPTIONAL GuiPrivate QuickTemplates2Private QuickPrivate)
set(QDASH_QT_LIBRARIES
    Qt6::Core Qt6::Quick Qt6::QuickControls2 Qt6::Qml
    Carboxyl::Carboxyl


    Qt6::QuickDialogs2)

if (QDASH_WEBVIEW)
    message(STATUS "[QDash] Building with web support")
    list(APPEND QDASH_QT_COMPONENTS WebEngineQuick)
    list(APPEND QDASH_QT_LIBRARIES Qt6::WebEngineQuick)
    list(APPEND QDASH_DEFINES QDASH_WEBVIEW)
endif()

if (QDASH_CAMVIEW)
    message(STATUS "[QDash] Building with camera support")
    list(APPEND QDASH_QT_COMPONENTS Multimedia MultimediaQuickPrivate)
    list(APPEND QDASH_QT_LIBRARIES Qt6::Multimedia Qt6::MultimediaQuickPrivate)
    list(APPEND QDASH_DEFINES QDASH_CAMVIEW)
endif()

if (QDASH_NETWORK)
    message(STATUS "[QDash] Building with network support")
    list(APPEND QDASH_QT_COMPONENTS Network)
    list(APPEND QDASH_QT_LIBRARIES Qt6::Network)
    list(APPEND QDASH_DEFINES QDASH_NETWORK)
endif()

if (QDASH_BUNDLE_QT)
    if (APPLE AND NOT IOS AND QDASH_CAMVIEW)
        AddJsonPackage(NAME ffmpeg MODULE)
        set(FFMPEG_DIR "${FFmpeg_SOURCE_DIR}")
    endif()

    AddQt(QDash-CI/Qt 6.11.1)

    if (APPLE OR NOT UNIX)
        set(QDASH_STATIC ON)
    endif()

    if (MSVC AND ARCHITECTURE_arm64)
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    endif()
endif()

if (QDASH_STATIC)
    include(StaticBuild)

    # static builds require explicitly provided zlib
    AddJsonPackage(zlib)
endif()

if (QDASH_STATIC OR IOS)
    # weirdness
    list(APPEND QDASH_QT_LIBRARIES Qt6::qmlshapesplugin)
endif()

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.8 REQUIRED)

if (Qt6_VERSION VERSION_GREATER_EQUAL 6.9)
    list(APPEND QDASH_QT_COMPONENTS QmlModels)
    list(APPEND QDASH_QT_LIBRARIES Qt6::QmlModels)
else()
    list(APPEND QDASH_QT_COMPONENTS LabsQmlModels)
    list(APPEND QDASH_QT_LIBRARIES Qt6::LabsQmlModels)
endif()

find_package(Qt6 REQUIRED
    COMPONENTS ${QDASH_QT_COMPONENTS}
    OPTIONAL_COMPONENTS ${QDASH_QT_OPTIONAL})

if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()

qt_standard_project_setup(REQUIRES 6.8)

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
