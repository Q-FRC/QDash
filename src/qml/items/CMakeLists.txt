set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlItems STATIC)
qt6_add_qml_module(QDashQmlItems
    URI items
    VERSION 1.0
    QML_FILES
        BetterMenu.qml
        BetterMenuBar.qml
        IconButton.qml
        MainScreen.qml
        RadialGauge.qml
        Tab.qml
        TitleBar.qml
        TopicView.qml
        ResizeAnchor.qml
        DoubleSpinBox.qml
    )
