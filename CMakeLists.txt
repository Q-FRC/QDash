cmake_minimum_required(VERSION 3.25)

project(QFRCDashboard VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC_SEARCH_PATHS "lib/ui/")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_TESTS "Build tests" ON)

set(QTCOMPONENTS Widgets MultimediaWidgets Charts)

if(WITH_TESTS)
    set(QTCOMPONENTS ${QTCOMPONENTS} Test)
endif()

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QTCOMPONENTS})

set(Dashboard_ORG_NAME "binex-dsk")
set(Dashboard_APP_NAME "QFRCDashboard")
set(Dashboard_GIT_REPO "https://github.com/binex-dsk/QFRCDashboard")

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/program_info/${Dashboard_APP_NAME}.rc)
    set(Dashboard_BUILD_PLATFORM "win32")
else()
    set(Dashboard_BUILD_PLATFORM "lin64")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Git
include(GetGitRevisionDescription)
git_get_exact_tag(Dashboard_GIT_TAG)
get_git_head_revision(Dashboard_GIT_REFSPEC Dashboard_GIT_COMMIT)

message(STATUS "Git commit: ${Dashboard_GIT_COMMIT}")
message(STATUS "Git tag: ${Dashboard_GIT_TAG}")
message(STATUS "Git refspec: ${Dashboard_GIT_REFSPEC}")

string(REPLACE "-beta" "." Dashboard_GIT_TAG_NUMERO ${Dashboard_GIT_TAG})

string(REPLACE "." " " Dashboard_SPLIT_VERSION ${Dashboard_GIT_TAG_NUMERO})

separate_arguments(Dashboard_SPLIT_VERSION)

list(GET Dashboard_SPLIT_VERSION 0 Dashboard_VERSION_MAJOR)
list(GET Dashboard_SPLIT_VERSION 1 Dashboard_VERSION_MINOR)
list(GET Dashboard_SPLIT_VERSION 2 Dashboard_VERSION_HOTFIX)

project(QFRCDashboard VERSION ${Dashboard_GIT_TAG_NUMERO} LANGUAGES CXX)

# Build Date
string(TIMESTAMP TODAY "%Y-%m-%d")
set(Dashboard_BUILD_TIMESTAMP "${TODAY}")

add_subdirectory("program_info")
add_subdirectory(lib)
find_package(ntcore)
find_package(Protobuf)

set(QAPPLICATION_CLASS QApplication)
add_subdirectory("SingleApplication")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QFRCDashboard
        MANUAL_FINALIZATION
        main.cpp
        ${RCS_FILE}
        lib/fields.qrc
        lib/theme/qrc/breeze.qrc
        lib/icons.qrc
    )
endif()

target_link_libraries(QFRCDashboard PRIVATE QFRCDashboardLib SingleApplication::SingleApplication Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Charts ntcore BuildConfig)

target_include_directories(QFRCDashboard PRIVATE "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
target_include_directories(QFRCDashboard PRIVATE lib/ui/)
target_include_directories(QFRCDashboard PRIVATE lib/include)
target_include_directories(QFRCDashboard PRIVATE ${_Dashboard_AUTOGEN}/include)
target_include_directories(QFRCDashboard PRIVATE lib/)

# courtesy of Cristian Adam
# enable CMake support for all targets (wpilib, protobuf, lib, tests...)
function(ccache_handle_debug_msvc)
    if (MSVC)
        foreach(config DEBUG RELWITHDEBINFO)
            foreach(lang C CXX)
                set(flags_var "CMAKE_${lang}_FLAGS_${config}")
                string(REPLACE "/Zi" "/Z7" ${flags_var} "${${flags_var}}")
                set(${flags_var} "${${flags_var}}" PARENT_SCOPE)
            endforeach()
        endforeach()
    endif()
endfunction()

ccache_handle_debug_msvc()

include(GNUInstallDirs)
install(TARGETS QFRCDashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/qt.conf\" \" \")"
    COMPONENT Runtime
)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

set_target_properties(QFRCDashboard PROPERTIES
    MACOSX_BUNDLE TRUE
)

if(UNIX AND APPLE)
    # Mac bundle settings
    set(MACOSX_BUNDLE_BUNDLE_NAME "${Dashboard_APP_NAME}")
    set(MACOSX_BUNDLE_INFO_STRING "${Dashboard_APP_NAME}: A high-performance, reliable dashboard for FRC use.")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.${Dashboard_ORG_NAME}.${Dashboard_APP_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${Dashboard_GIT_TAG_NUMERO}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${Dashboard_GIT_TAG_NUMERO}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${Dashboard_GIT_TAG_NUMERO}")
    set(MACOSX_BUNDLE_ICON_FILE ${Dashboard_APP_NAME}.icns)
    set(MACOSX_BUNDLE_COPYRIGHT "Copyleft 2023-2024 ${Dashboard_ORG_NAME}")

    # install as bundle
    set(INSTALL_BUNDLE "full")
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QFRCDashboard)
endif()
