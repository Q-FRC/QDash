set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlDialogs STATIC)
qt6_add_qml_module(QDashQmlDialogs
    URI QDash.Dialogs
    VERSION 1.0
    QML_FILES
        AboutDialog.qml
        NotificationPopup.qml
        TextDialog.qml
        remote/RemoteLayout.qml
        remote/RemoteLayoutsDialog.qml
        settings/AppearanceComboBox.qml
        settings/AppearanceTab.qml
        settings/MiscTab.qml
        settings/ServerTab.qml
        settings/SettingsDialog.qml
        settings/SettingsTabButton.qml
        settings/WindowTab.qml
        tab/TabCloseDialog.qml
        tab/TabDialog.qml
        tab/TabNameDialog.qml
        NativeDialog.qml)

QDashModule(QDashQmlDialogs)
