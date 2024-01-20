cmake_minimum_required(VERSION 3.25)

project(QFRCDashboard VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

option(USE_SYSTEM_NTCORE "Use system wpilib installation" ON)
option(USE_SYSTEM_PROTOBUF "Use system protobuf installation" ON)

if( ${USE_SYSTEM_NTCORE})
    find_package(ntcore)
    if( NOT ${ntcore_FOUND} )
        set(USE_SYSTEM_NTCORE OFF)
    endif()
endif()

if( ${USE_SYSTEM_PROTOBUF})
    find_package(Protobuf)
    if( NOT ${Protobuf_FOUND} )
        set(USE_SYSTEM_PROTOBUF OFF)
    endif()
endif()

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets MultimediaWidgets Charts)
add_subdirectory(3rd_party EXCLUDE_FROM_ALL)

set(PROJECT_SOURCES
        src/main.cpp
        src/Globals.cpp
        src/MainWindow.cpp

        src/widgets/BaseWidget.cpp
        src/widgets/DoubleDisplayWidget.cpp
        src/widgets/TabWidget.cpp
        src/widgets/BooleanDisplayWidget.cpp
        src/widgets/StringDisplayWidget.cpp
        src/widgets/TextWidget.cpp
        src/widgets/BooleanCheckboxWidget.cpp
        src/widgets/DoubleDialWidget.cpp
        src/widgets/StringChooserWidget.cpp
        src/widgets/CameraViewWidget.cpp
        src/widgets/EnumWidget.cpp
        src/widgets/IntegerDisplayWidget.cpp
        src/widgets/IntegerDialWidget.cpp
        src/widgets/FieldWidget.cpp
        src/widgets/SendableFieldWidget.cpp
        src/widgets/CommandWidget.cpp
        src/widgets/GraphWidget.cpp

        src/misc/ShapedFrame.cpp
        src/misc/BetterDial.cpp
        src/misc/FieldImage.cpp
        src/dialogs/TabMaxSizeDialog.cpp

        src/stores/TopicStore.cpp
        src/stores/TypeStore.cpp
        src/stores/FilterStore.cpp

        src/dialogs/CameraSelectionDialog.cpp
        src/dialogs/NewWidgetTreeDialog.cpp
        src/dialogs/NTSettingsDialog.cpp
        src/dialogs/WidgetDialogGenerator.cpp
        src/dialogs/TabMaxSizeDialog.cpp

        # =======================================

        include/Globals.h
        include/MainWindow.h

        include/widgets/BaseWidget.h
        include/widgets/DoubleDisplayWidget.h
        include/widgets/TabWidget.h
        include/widgets/BooleanDisplayWidget.h
        include/widgets/StringDisplayWidget.h
        include/widgets/TextWidget.h
        include/widgets/BooleanCheckboxWidget.h
        include/widgets/DoubleDialWidget.h
        include/widgets/StringChooserWidget.h
        include/widgets/CameraViewWidget.h
        include/widgets/EnumWidget.h
        include/widgets/IntegerDisplayWidget.h
        include/widgets/IntegerDialWidget.h
        include/widgets/FieldWidget.h
        include/widgets/SendableFieldWidget.h
        include/widgets/CommandWidget.h
        include/widgets/GraphWidget.h

        include/misc/ShapedFrame.h
        include/misc/BetterDial.h
        include/misc/FieldImage.h

        include/stores/TopicStore.h
        include/stores/TypeStore.h
        include/stores/FilterStore.h

        include/dialogs/CameraSelectionDialog.h
        include/dialogs/NewWidgetTreeDialog.h
        include/dialogs/NTSettingsDialog.h
        include/dialogs/WidgetDialogGenerator.h
        include/dialogs/TabMaxSizeDialog.h
)

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/program_info/QFRCDashboard.rc)
    set(Dashboard_BUILD_PLATFORM "win32")
else()
    set(Dashboard_BUILD_PLATFORM "lin64")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QFRCDashboard
        MANUAL_FINALIZATION
        fields.qrc
        theme/qrc/breeze.qrc
        ${RCS_FILE}
        ${PROJECT_SOURCES}
        include/misc/ChartView.h src/misc/ChartView.cpp
        include/stores/CameraStore.h src/stores/CameraStore.cpp
        include/dialogs/PreferencesDialog.h src/dialogs/PreferencesDialog.cpp
    )
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(Dashboard_ORG_NAME "binex-dsk")
set(Dashboard_APP_NAME "QFRCDashboard")
set(Dashboard_GIT_REPO "https://github.com/binex-dsk/QFRCDashboard")

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

# Build Date
string(TIMESTAMP TODAY "%Y-%m-%d")
set(Dashboard_BUILD_TIMESTAMP "${TODAY}")

set(QAPPLICATION_CLASS QApplication)
add_subdirectory("SingleApplication")

add_subdirectory("program_info")
add_subdirectory("buildconfig")

target_link_libraries(QFRCDashboard PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Charts ntcore SingleApplication::SingleApplication BuildConfig)

target_include_directories(QFRCDashboard PRIVATE "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
target_include_directories(QFRCDashboard PRIVATE include/)

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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QFRCDashboard)
endif()
