set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlMain STATIC)
qt6_add_qml_module(QDashQmlMain
    URI QDash.Main
    VERSION 1.0
    QML_FILES Main.qml
    )
