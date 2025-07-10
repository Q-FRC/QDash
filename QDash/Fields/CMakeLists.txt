set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlFields STATIC)
qt6_add_qml_module(QDashQmlFields
    URI QDash.Fields
    VERSION 1.0
    QML_FILES
        BetterComboBox.qml
        BetterDoubleSpinBox.qml
        BetterMenuItem.qml
        BetterSpinBox.qml
        BetterTextField.qml
        FieldFooter.qml
        SearchableComboBox.qml
    )
