# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

# Build Configuraton
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/BuildConfig.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.cpp")

add_library(BuildConfig STATIC
    BuildConfig.h
    ${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.cpp
)

target_link_libraries(BuildConfig Qt6::Core)
target_include_directories(BuildConfig PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
