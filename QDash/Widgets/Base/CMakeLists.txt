set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgetsBase STATIC)
qt6_add_qml_module(QDashQmlWidgetsBase
    URI QDash.Widgets.Base
    VERSION 1.0
    QML_FILES
        BaseWidget.qml
        PrimitiveWidget.qml
        SendableWidget.qml
    )
