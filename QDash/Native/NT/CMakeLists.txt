set(CMAKE_AUTOMOC ON)

qt_add_library(QDashNt STATIC)
qt6_add_qml_module(QDashNt
    URI QDash.Native.NT
    VERSION 1.0
    SOURCES
    TopicStore.cpp
    TopicStore.h)

QDashModule(QDashNt)
