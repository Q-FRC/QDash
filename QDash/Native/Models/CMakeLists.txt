set(CMAKE_AUTOMOC ON)

qt_add_library(QDashModels STATIC)
qt6_add_qml_module(QDashModels
    URI QDash.Native.Models
    VERSION 1.0
    SOURCES


    RemoteLayoutModel.cpp
    RemoteLayoutModel.h
    TabListModel.cpp
    TabListModel.h
    TabWidgetsModel.cpp
    TabWidgetsModel.h
    TopicListModel.cpp
    TopicListModel.h
    GenericMapModel.cpp
    GenericMapModel.h
    SOURCES WindowModel.h WindowModel.cpp)

QDashModule(QDashModels)
target_link_libraries(QDashModels PRIVATE ntcore)
