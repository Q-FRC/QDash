set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgets STATIC)
qt6_add_qml_module(QDashQmlWidgets
    URI widgets
    VERSION 1.0
    QML_FILES
        misc/CameraView.qml
        misc/ErrorsWidget.qml
        misc/WebView.qml

        primitive/BoolWidget.qml
        primitive/ColorTextWidget.qml
        primitive/ColorWidget.qml
        primitive/ReefDisplay.qml
        primitive/ShapeHandler.qml
        primitive/StringDisplay.qml
        primitive/TextWidget.qml

        primitive/number/DoubleDialWidget.qml
        primitive/number/DoubleDisplay.qml
        primitive/number/DoubleGaugeWidget.qml
        primitive/number/DoubleProgressBar.qml
        primitive/number/DoubleWidget.qml
        primitive/number/IntDialWidget.qml
        primitive/number/IntDisplay.qml
        primitive/number/IntGaugeWidget.qml
        primitive/number/IntWidget.qml
        primitive/number/MatchTime.qml

        sendable/Command.qml
        sendable/FMSInfo.qml
        sendable/Field2d.qml
        sendable/StringChooser.qml
    )
