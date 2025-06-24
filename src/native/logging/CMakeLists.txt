set(CMAKE_AUTOMOC ON)

qt_add_library(QDashLogging STATIC)
qt6_add_qml_module(QDashLogging
    URI logging
    VERSION 1.0
    SOURCES
    Logger.h
    Logger.cpp
)


target_link_libraries(QDashLogging
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)
