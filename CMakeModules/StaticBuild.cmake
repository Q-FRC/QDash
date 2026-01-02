# SPDX-FileCopyrightText: Copyright 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

## StaticBuild ##

# This module sets some things up to make static builds on MinGW
# and macOS easier. Currently does not support any other platforms,
# but this is subject to change.

# Aside from this, you will likely want to do some handling on your own
# to handle e.g. bundled dependencies, using static suffixes for libraries
# that have them (such as zstd), etc.

# Additionally, if you're using a static Qt, call static_qt_link on your
# Qt-linking targets for MinGW. Trust me, you'll need it ;)

#[[
Also, for Qt, make sure you include the platform plugins:

#ifdef QT_STATICPLUGIN
    #include <QtPlugin>

    #if defined(_WIN32)
        Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
    #elif defined(__APPLE__)
        Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
    #endif
#endif

]]

# lol
set(Boost_USE_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)

## find .a libs first (static, usually)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

## some libraries use CMAKE_IMPORT_LIBRARY_SUFFIX e.g. Harfbuzz ##
set(CMAKE_IMPORT_LIBRARY_SUFFIX ".a")

# Unfortunately, using .a as the suffix ALSO includes dll.a
# libraries... so we need a quick hook to reject those libraries.
if (MINGW)
    function(find_library var)
        # also skip previously-found libraries cuz... yaknow
        # UNLESS they are dll.a :{
        if (${var} AND NOT ${var} MATCHES "dll\\.a$")
            return()
        endif()

        _find_library(${var} ${ARGN})
        if (${var})
            get_filename_component(lib_name "${${var}}" NAME)
            if (lib_name MATCHES "dll\\.a$")
                unset(${var} CACHE)
                set(${var} "${var}-NOTFOUND" CACHE INTERNAL "" FORCE)
            endif()
        endif()
    endfunction()
endif()

# ONLY NEEDED FOR MINGW!
function(static_qt_link target)
    macro(extra_libs)
        foreach(lib ${ARGN})
            find_library(${lib}_LIBRARY ${lib} REQUIRED)
            target_link_libraries(${target} PRIVATE ${${lib}_LIBRARY})
        endforeach()
    endmacro()

    # I am constantly impressed at how ridiculously stupid the linker is
    # NB: yes, we have to put them here twice. I have no idea why

    # libtiff.a
    extra_libs(tiff jbig bz2 lzma deflate jpeg tiff)

    # libfreetype.a
    extra_libs(freetype bz2 Lerc brotlidec brotlicommon freetype)

    # libharfbuzz.a
    extra_libs(harfbuzz graphite2)
endfunction()

# Some static linker options and such
add_compile_definitions(QT_STATICPLUGIN)

# macos doesn't even let you make static executables btw
if (MINGW)
    add_compile_options(-static)

    add_link_options(-static -lpthread)
    add_link_options(-static-libgcc -static-libstdc++)
endif()
