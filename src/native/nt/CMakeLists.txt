set(CMAKE_AUTOMOC ON)

qt_add_library(QDashNt STATIC)
qt6_add_qml_module(QDashNt
    URI nt
    VERSION 1.0
    SOURCES
        TopicStore.cpp
        TopicStore.h
    )

target_link_libraries(QDashNt
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)

