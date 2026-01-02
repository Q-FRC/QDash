set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlFields STATIC)
qt6_add_qml_module(QDashQmlFields
    URI QDash.Fields
    VERSION 1.0
    # TODO: Move to Carboxyl
    QML_FILES SearchableComboBox.qml)

QDashModule(QDashQmlFields)
