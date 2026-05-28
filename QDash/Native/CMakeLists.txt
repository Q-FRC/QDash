# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
add_compile_definitions("${QDASH_DEFINES}")

add_subdirectory(buildconfig)
add_subdirectory(Helpers)
add_subdirectory(Logging)
add_subdirectory(Managers)
add_subdirectory(Models)
add_subdirectory(NT)

# Main executable
add_subdirectory(main)
