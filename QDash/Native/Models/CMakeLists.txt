# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashModels STATIC)
qt6_add_qml_module(QDashModels
    URI QDash.Native.Models
    VERSION 1.0
    SOURCES


    FieldImageModel.cpp
    FieldImageModel.h
    RemoteLayoutModel.cpp
    RemoteLayoutModel.h
    TabListModel.cpp
    TabListModel.h
    TabWidgetsModel.cpp
    TabWidgetsModel.h
    TopicListModel.cpp
    TopicListModel.h
    GenericMapModel.cpp
    GenericMapModel.h)

QDashModule(QDashModels)
target_link_libraries(QDashModels PRIVATE ntcore)
