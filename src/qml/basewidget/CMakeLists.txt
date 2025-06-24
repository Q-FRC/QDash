set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlBaseWidget STATIC)
qt6_add_qml_module(QDashQmlBaseWidget
    URI basewidget
    VERSION 1.0
    QML_FILES
        BaseWidget.qml
        PrimitiveWidget.qml
        SendableWidget.qml
    )
