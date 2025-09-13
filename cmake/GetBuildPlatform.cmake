# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

if (WIN32)
  set(QDASH_BUILD_PLATFORM "Windows")
elseif (APPLE)
  set(QDASH_BUILD_PLATFORM "macOS")
else()
  set(QDASH_BUILD_PLATFORM "Linux")
endif()

set(QDASH_BUILD_PLATFORM ${QDASH_BUILD_PLATFORM} PARENT_SCOPE)