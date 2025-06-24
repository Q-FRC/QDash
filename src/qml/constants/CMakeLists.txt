set(CMAKE_AUTOMOC ON)

set_source_files_properties(Constants.qml
    PROPERTIES
    QT_QML_SINGLETON_TYPE true
)

qt_add_library(QDashQmlConstants STATIC)
qt6_add_qml_module(QDashQmlConstants
    URI constants
    VERSION 1.0
    QML_FILES
        Constants.qml
    )
