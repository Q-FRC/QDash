# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

## Executable ##
qt_add_executable(${QDASH_NAME}
    main.cpp
    QDashApplication.h QDashApplication.cpp

    ${QRC_FILES}
    ${RCS_FILE})

## QML module ##
set_source_files_properties(CustomThemes.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt6_add_qml_module(${QDASH_NAME}
    URI QDash.Main
    VERSION 1.0
    QML_FILES
        Main.qml
        CustomThemes.qml
    RESOURCES
        hannahmontana.jpg
    RESOURCE_PREFIX "/qt/qml")

## Plugins/Libraries ##
target_link_libraries(${QDASH_NAME}
    PRIVATE
    ${QDASH_QT_LIBRARIES}

    QDashQml
    QDashBackend)

#############################
## Platform-Specific Stuff ##
#############################

## MacOS ##
set_target_properties(${QDASH_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE)

if(UNIX AND APPLE)
    # Mac bundle settings
    set(MACOSX_BUNDLE_BUNDLE_NAME "${QDASH_NAME}")
    set(MACOSX_BUNDLE_INFO_STRING "${QDASH_NAME}: A high-performance, reliable dashboard for FRC use.")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.${QDASH_ORG}.${QDASH_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_ICON_FILE ${QDASH_NAME})
    set(MACOSX_BUNDLE_COPYRIGHT "Copyleft 2023-2026 ${QDASH_ORG}")

    # make assets.car myself because xcode sucks
    if (IOS)
        set(qdash_icon ${CMAKE_SOURCE_DIR}/dist/${QDASH_NAME}.icon)

        set_source_files_properties(${qdash_icon} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
            XCODE_FILE_ATTRIBUTES "CodeSignOnCopy")

        target_sources(${QDASH_NAME} PRIVATE ${qdash_icon})
    else()
        set(qdash_assets ${CMAKE_SOURCE_DIR}/dist/Assets.car)
        set(qdash_icns ${CMAKE_SOURCE_DIR}/dist/${QDASH_NAME}.icns)

        set_source_files_properties(${qdash_icns} ${qdash_assets} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
            XCODE_FILE_ATTRIBUTES "CodeSignOnCopy")

        target_sources(${QDASH_NAME} PRIVATE ${qdash_assets} ${qdash_icns})
    endif()

    set_target_properties(${QDASH_NAME} PROPERTIES
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "${QDASH_NAME}"
        MACOSX_BUNDLE_ICON_FILE ${QDASH_NAME})

    # install as bundle
    set(INSTALL_BUNDLE "full")

    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED ON)
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED ON)
endif()

## Windows ##
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ${QDASH_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
endif()

## iOS ##
if (IOS)
    qt_add_ios_ffmpeg_libraries(${QDASH_NAME})
endif()

## Installation ##
include(GNUInstallDirs)
install(TARGETS ${QDASH_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

