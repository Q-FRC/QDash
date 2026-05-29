# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

add_subdirectory(BuildConfig)
add_subdirectory(Helpers)
add_subdirectory(Services)
add_subdirectory(Models)

add_library(QDashBackend INTERFACE)
target_link_libraries(QDashBackend INTERFACE
    QDashHelpers
    QDashServices
    QDashModels

    BuildConfig)
