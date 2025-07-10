set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgetsPrimitive STATIC)
qt6_add_qml_module(QDashQmlWidgetsPrimitive
    URI QDash.Widgets.Primitive
    VERSION 1.0
    QML_FILES
        BoolWidget.qml
        ColorTextWidget.qml
        ColorWidget.qml
        ReefDisplay.qml
        ShapeHandler.qml
        StringDisplay.qml
        TextWidget.qml

        number/DoubleDialWidget.qml
        number/DoubleDisplay.qml
        number/DoubleGaugeWidget.qml
        number/DoubleProgressBar.qml
        number/DoubleWidget.qml
        number/IntDialWidget.qml
        number/IntDisplay.qml
        number/IntGaugeWidget.qml
        number/IntWidget.qml
        number/MatchTime.qml
    )
