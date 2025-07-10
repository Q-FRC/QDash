set(CMAKE_AUTOMOC ON)

qt_add_library(QDashHelpers STATIC)
qt6_add_qml_module(QDashHelpers
    URI QDash.Native.Helpers
    VERSION 1.0
    SOURCES
    CompileDefinitions.h
    MetaObjectHelper.h
    NotificationHelper.cpp NotificationHelper.h
    PlatformHelper.cpp PlatformHelper.h
)

target_link_libraries(QDashHelpers
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)

