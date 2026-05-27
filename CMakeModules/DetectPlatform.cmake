# SPDX-FileCopyrightText: Copyright 2025 crueter
# SPDX-License-Identifier: LGPL-3.0-or-later

## DetectPlatform ##

# This is a small helper that sets PLATFORM_<platform> variables for various
# operating systems and distributions. Note that Apple, Windows, Android, etc.
# are not covered, as CMake already does that for us.

# It also sets CXX_<compiler> for the C++ compiler.

# Furthermore, some platforms have really silly requirements/quirks, so this
# also does a few of those.

# This module contains contributions from the Eden Emulator Project,
# notably from crueter and Lizzie.

# Platforms
if (${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
    set(PLATFORM_SUN ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(PLATFORM_FREEBSD ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
    set(PLATFORM_OPENBSD ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "NetBSD")
    set(PLATFORM_NETBSD ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "DragonFly")
    set(PLATFORM_DRAGONFLYBSD ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Haiku")
    set(PLATFORM_HAIKU ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM_LINUX ON)
endif()

# Compilers
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CXX_CLANG ON)
    if (MSVC)
        set(CXX_CLANG_CL ON)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CXX_GCC ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CXX_CL ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    set(CXX_ICC ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CXX_APPLE ON)
endif()

# https://gitlab.kitware.com/cmake/cmake/-/merge_requests/11112
# This works totally fine on MinGW64, but not CLANG{,ARM}64
if(MINGW AND CXX_CLANG)
    set(CMAKE_SYSTEM_VERSION 10.0.0)
endif()

# This saves a truly ridiculous amount of time during linking
# In my tests, without this, Eden takes 2 mins, with this, it takes 3-5 seconds
# or on GitHub Actions, 10 minutes -> 3 seconds
if (MINGW)
    set(MINGW_FLAGS "-Wl,--strip-all -Wl,--gc-sections")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE
        "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${MINGW_FLAGS}")
endif()
