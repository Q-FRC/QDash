set(CMAKE_AUTOMOC ON)

qt_add_library(QDashManagers STATIC)
qt6_add_qml_module(QDashManagers
    URI QDash.Native.Managers
    VERSION 1.0
    SOURCES
    ConnManager.cpp ConnManager.h
    SettingsManager.cpp SettingsManager.h
)


target_link_libraries(QDashManagers
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)
