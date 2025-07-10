set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlConfig STATIC)
qt6_add_qml_module(QDashQmlConfig
    URI QDash.Config
    VERSION 1.0
    QML_FILES
        BaseConfigDialog.qml
        ColorField.qml
        LabeledCheckbox.qml
        LabeledComboBox.qml
        LabeledDoubleSpinBox.qml
        LabeledIndexComboBox.qml
        LabeledSpinBox.qml
        LabeledTextField.qml
        SectionHeader.qml
        TitleComponent.qml
)
