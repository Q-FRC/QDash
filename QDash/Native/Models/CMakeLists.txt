set(CMAKE_AUTOMOC ON)

qt_add_library(QDashModels STATIC)
qt6_add_qml_module(QDashModels
    URI QDash.Native.Models
    VERSION 1.0
    SOURCES
    AccentsListModel.cpp
    AccentsListModel.h
    RemoteLayoutModel.cpp
    RemoteLayoutModel.h
    TabListModel.cpp
    TabListModel.h
    TabWidgetsModel.cpp
    TabWidgetsModel.h
    TopicListModel.cpp
    TopicListModel.h
    SOURCES GenericMapModel.h GenericMapModel.cpp
)

target_link_libraries(QDashModels
    PRIVATE
    Qt6::Gui
    Qt6::Quick
)

