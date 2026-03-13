# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(qdash_icon ${CMAKE_SOURCE_DIR}/dist/${QDASH_NAME}.icns)

qt_add_executable(${QDASH_NAME}
    main.cpp
    ${QRC_FILES}
    ${RCS_FILE}
    ${qdash_icon}
    QDashApplication.h QDashApplication.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
add_compile_definitions("${QDASH_DEFINES}")

add_subdirectory(buildconfig)
add_subdirectory(Helpers)
add_subdirectory(Logging)
add_subdirectory(Managers)
add_subdirectory(Misc)
add_subdirectory(Models)
add_subdirectory(NT)

set(BASE_PLUGINS
    QDashHelpers
    QDashLogging
    QDashManagers
    QDashMisc
    QDashModels
    QDashNt)

foreach(PLUGIN ${BASE_PLUGINS})
    set(PLUGINS ${PLUGINS} ${PLUGIN}plugin ${PLUGIN})
endforeach()

target_link_libraries(${QDASH_NAME}
    PRIVATE
    BuildConfig
    ntcore

    ${QDASH_QT_LIBRARIES}
    ${PLUGINS}
    QDashQml)

set_target_properties(${QDASH_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE)

include(GNUInstallDirs)
install(TARGETS ${QDASH_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if (QDASH_STATIC AND MINGW)
    static_qt_link(${QDASH_NAME})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ${QDASH_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
endif()

if (IOS AND Qt6_VERSION VERSION_GREATER_EQUAL 6.8)
    qt_add_ios_ffmpeg_libraries(${QDASH_NAME})
endif()

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

    set_source_files_properties(${qdash_icon} PROPERTIES
           MACOSX_PACKAGE_LOCATION "Resources")

    # install as bundle
    set(INSTALL_BUNDLE "full")

    if (IOS)
        set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED ON)
        set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED ON)
    endif()
endif()
