# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

if (QDASH_NETWORK)
    set(extra_src RemoteLayoutModel.cpp RemoteLayoutModel.h)
    set(extra_libs Qt6::Network)
endif()

qt_add_library(QDashModels STATIC)
qt6_add_qml_module(QDashModels
    URI QDash.Backend.Models
    VERSION 1.0
    SOURCES
        TabListModel.cpp TabListModel.h
        TabWidgetsModel.cpp TabWidgetsModel.h
        TopicListModel.cpp TopicListModel.h
        GenericMapModel.cpp GenericMapModel.h
        ${extra_src})

target_link_libraries(QDashModels PRIVATE
    Qt6::Core Qt6::Qml Qt6::Gui
    QDashModelsplugin
    ${extra_libs})
