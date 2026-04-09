# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(Qt6 OPTIONAL_COMPONENTS QuickTest)

if(NOT Qt6QuickTest_FOUND)
    message(STATUS "[QDash] Qt6QuickTest not found, skipping widget tests")
    return()
endif()

enable_testing()

set(CMAKE_AUTOMOC ON)

add_executable(tst_Widgets tst_Widgets.cpp)

# Expose QDash/Native headers so that modules compiled into this binary can
# find internal includes (e.g. NT/TopicStore.h, Misc/Globals.h).
target_include_directories(tst_Widgets PRIVATE
    "${CMAKE_SOURCE_DIR}/QDash/Native"
    "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>"
)

target_link_libraries(tst_Widgets
    PRIVATE
    Qt6::QuickTest
    Qt6::Quick
    Qt6::Qml
    Qt6::Test
    Qt6::Core

    BuildConfig

    # NT / native infrastructure
    ntcore
    QDashMisc
    QDashMiscplugin
    QDashModels
    QDashModelsplugin
    QDashNt
    QDashNtplugin
    QDashLogging
    QDashLoggingplugin

    # Compile-time helpers (provides CompileDefinitions QML singleton)
    QDashHelpers
    QDashHelpersplugin

    # QML UI modules used by the widget hierarchy
    QDashQmlWidgets
    QDashQmlWidgetsBaseplugin
    QDashQmlWidgetsBase
    QDashQmlWidgetsPrimitiveplugin
    QDashQmlWidgetsPrimitive
    QDashQmlWidgetsMiscplugin
    QDashQmlWidgetsMisc
    QDashQmlWidgetsSendableplugin
    QDashQmlWidgetsSendable

    QDashQmlItems
    QDashQmlItemsplugin

    QDashQmlConfig
    QDashQmlConfigplugin

    QDashQmlFields
    QDashQmlFieldsplugin
)

target_compile_definitions(tst_Widgets PRIVATE
    QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/quick/widgets"
)

add_test(
    NAME tst_Widgets
    COMMAND tst_Widgets -input "${CMAKE_CURRENT_SOURCE_DIR}/quick/widgets"
)

set_tests_properties(tst_Widgets PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)
