# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

set(QDASH_NATIVE_INCLUDE "${CMAKE_SOURCE_DIR}/QDash/Native")

# Common libraries required by all test executables.
# QDashNt is included because QDashModels contains TopicListModel which references
# TopicStore symbols from QDashNt.
set(QDASH_TEST_LINK_LIBRARIES
    Qt6::Test
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Widgets
    BuildConfig
    QDashModels
    QDashLogging
    QDashManagers
    QDashMisc
    QDashNt
    ntcore)

function(QDashTest name)
    qt_add_executable(${name} ${ARGN})

    target_include_directories(${name} PRIVATE "${QDASH_NATIVE_INCLUDE}")

    target_link_libraries(${name} PRIVATE ${QDASH_TEST_LINK_LIBRARIES})

    add_test(NAME ${name} COMMAND ${name})
endfunction()

QDashTest(TestTabWidgetsModel TestTabWidgetsModel.cpp)
QDashTest(TestGenericMapModel TestGenericMapModel.cpp)
QDashTest(TestConnManager     TestConnManager.cpp)
QDashTest(TestTabListModel    TestTabListModel.cpp)
QDashTest(TestAccentsListModel TestAccentsListModel.cpp)
QDashTest(TestSettingsManager TestSettingsManager.cpp)

# ---------------------------------------------------------------------------
# Qt Quick (QML) tests
# ---------------------------------------------------------------------------
if(Qt6QuickTest_FOUND)
    function(QDashQuickTest name)
        qt_add_executable(${name} ${name}.cpp)

        target_include_directories(${name} PRIVATE "${QDASH_NATIVE_INCLUDE}")

        # Compile-time constant so the harness can add the QML module output
        # directory to the QML engine's import path at runtime.
        target_compile_definitions(${name} PRIVATE
            "QDASH_QML_PATH=\"${CMAKE_BINARY_DIR}/qml\"")

        target_link_libraries(${name} PRIVATE
            Qt6::QuickTest
            Qt6::Quick
            Qt6::Qml
            Qt6::Core
            Qt6::Widgets
            BuildConfig
            # QML backing libs + their plugin counterparts (required for static builds)
            QDashQmlItemsplugin   QDashQmlItems
            QDashQmlConfigplugin  QDashQmlConfig
            Carboxyl::Carboxyl
            ntcore)

        add_test(NAME ${name}
            COMMAND ${name} -input "${CMAKE_CURRENT_SOURCE_DIR}/quick")

        set_tests_properties(${name} PROPERTIES
            ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
    endfunction()

    QDashQuickTest(tst_QmlComponents)
else()
    message(STATUS "[QDash tests] Qt6::QuickTest not found – skipping QML tests")
endif()
