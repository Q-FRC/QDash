set(CMAKE_AUTOMOC ON)

qt_add_library(QDashMisc STATIC)
qt6_add_qml_module(QDashMisc
    URI misc
    VERSION 1.0
    SOURCES
    Constants.cpp
    Constants.h
    Flags.h
    Globals.cpp
    Globals.h
    MaskedMouseArea.cpp
    MaskedMouseArea.h
)

target_link_libraries(QDashMisc
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)
