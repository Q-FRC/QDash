# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashQmlWidgets STATIC)

qt_add_qml_module(QDashQmlWidgets
    URI QDash.Widgets
    VERSION 1.0
    QML_FILES
        base/BaseWidget.qml
        base/PrimitiveWidget.qml
        base/SendableWidget.qml
        base/ResizeAnchor.qml
        base/ResizeComponent.qml
        base/ShapeHandler.qml

        sendable/Command.qml
        sendable/FMSInfo.qml
        sendable/Field2d.qml
        sendable/StringChooser.qml

        bool/BoolWidget.qml
        bool/ColorWidget.qml

        string/ColorTextWidget.qml
        string/StringDisplay.qml
        string/TextWidget.qml

        double/DoubleDialWidget.qml
        double/DoubleDisplay.qml
        double/DoubleGaugeWidget.qml
        double/DoubleProgressBar.qml
        double/DoubleWidget.qml
        double/MatchTime.qml

        int/IntDialWidget.qml
        int/IntDisplay.qml
        int/IntWidget.qml)

if (QDASH_CAMVIEW)
    # TODO: Reduce duped code here
    qt_target_qml_sources(QDashQmlWidgets
        QML_FILES
            misc/CameraView.qml
            misc/UrlCameraView.qml)
endif()

if (QDASH_WEBVIEW)
    qt_target_qml_sources(QDashQmlWidgets
        QML_FILES
            misc/WebView.qml)
endif()

get_target_property(files QDashQmlWidgets QT_QML_MODULE_QML_FILES)
foreach(file ${files})
    # Ignore base types
    if (file MATCHES "base")
        continue()
    endif()

    set_property(DIRECTORY APPEND PROPERTY
        CMAKE_CONFIGURE_DEPENDS
        "${file}")

    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" file_rel "${file}")
    string(REPLACE "/" ";" file_list "${file_rel}")

    list(GET file_list 0 type)

    file(READ ${file} content)

    # Role strings
    string(REGEX MATCH "roleString: \"([a-zA-Z0-9 ]*)\"" rs_out ${content})
    if (rs_out)
        string(REPLACE "roleString: " "" rs_tmp ${rs_out})
        string(REPLACE "\"" "" role_string ${rs_tmp})
    else()
        message(FATAL_ERROR "File ${file_rel} does not "
            "have a roleString property defined!")
    endif()

    # now construct DelegateChoice
    list(GET file_list 1 filename)

    # we need the base name without .qml
    string(REPLACE "." ";" filename_lst ${filename})
    list(GET filename_lst 0 basename)

    string(CONFIGURE [=[
    DelegateChoice {
        roleValue: "@role_string@"
        @basename@ {}
    }

]=] delegate_choice @ONLY)

    string(APPEND DELEGATES ${delegate_choice})

    # The rest of this is only needed for typed widgets.
    # Misc and sendable don't need any of this
    if (file MATCHES "sendable" OR file MATCHES "misc")
        continue()
    endif()

    # Read-only
    string(REGEX MATCH "readOnly: ([a-z]*)" ro_out ${content})
    if (ro_out)
        string(REPLACE "readOnly: " "" readonly ${ro_out})
    else()
        set(readonly false)
    endif()

    # Labels
    string(REGEX MATCH "widgetLabel: \"([a-zA-Z0-9 ]*)\"" label_out ${content})
    if (label_out)
        string(REPLACE "widgetLabel: " "" label_tmp ${label_out})
        string(REPLACE "\"" "" label ${label_tmp})
    else()
        message(FATAL_ERROR "File ${file_rel} does not "
            "have a widgetLabel property defined!")
    endif()

    # TODO: Generate type map for QML and C++
    # TODO: Default {im,}mutable types for TopicListModel?

    # construct widget type map
    # TODO: LABELS
    string(CONFIGURE [=[
        {"type": "@type@", "roleString": "@role_string@",
            "readOnly": @readonly@, "label": "@label@"},
]=] type_map @ONLY)
    string(APPEND TYPE_MAP ${type_map})
endforeach()

configure_file(WidgetDelegate.qml.in ${CMAKE_CURRENT_SOURCE_DIR}/WidgetDelegate.qml)
configure_file(WidgetTypeMap.qml.in ${CMAKE_CURRENT_SOURCE_DIR}/WidgetTypeMap.qml)

set_source_files_properties(WidgetTypeMap.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_target_qml_sources(QDashQmlWidgets
    QML_FILES
    WidgetDelegate.qml
    WidgetTypeMap.qml)
