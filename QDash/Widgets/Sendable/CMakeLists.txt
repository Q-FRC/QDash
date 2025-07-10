set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgetsSendable STATIC)
qt6_add_qml_module(QDashQmlWidgetsSendable
    URI QDash.Widgets.Sendable
    VERSION 1.0
    QML_FILES
        Command.qml
        FMSInfo.qml
        Field2d.qml
        StringChooser.qml
    )
