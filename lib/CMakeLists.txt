cmake_minimum_required(VERSION 3.25)

project(QFRCDashboardLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC_SEARCH_PATHS "ui/")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

option(USE_SYSTEM_NTCORE "Use system wpilib installation" ON)
option(USE_SYSTEM_PROTOBUF "Use system protobuf installation" ON)

if( ${USE_SYSTEM_NTCORE})
    find_dependency(ntcore)
    if( NOT ${ntcore_FOUND} )
        set(USE_SYSTEM_NTCORE OFF)
    endif()
endif()

if( ${USE_SYSTEM_PROTOBUF})
    find_dependency(Protobuf)
    if( NOT ${Protobuf_FOUND} )
        set(USE_SYSTEM_PROTOBUF OFF)
    endif()
endif()

find_dependency(QT NAMES Qt6 REQUIRED)
find_dependency(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets MultimediaWidgets Charts)
add_subdirectory(3rd_party EXCLUDE_FROM_ALL)

set(PROJECT_SOURCES
        src/Globals.cpp
        src/MainWindow.cpp
        src/Constants.cpp

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
        src/widgets/FMSInfoWidget.cpp
        src/widgets/SwerveWidget.cpp

        src/misc/ShapedFrame.cpp
        src/misc/BetterDial.cpp
        src/misc/FieldImage.cpp
        src/misc/ChartView.cpp
        src/misc/GridLineWidget.cpp
        src/misc/SwerveTrain.cpp

        src/stores/TopicStore.cpp
        src/stores/TypeStore.cpp
        src/stores/FilterStore.cpp
        src/stores/CameraStore.cpp

        src/dialogs/CameraSelectionDialog.cpp
        src/dialogs/NewWidgetTreeDialog.cpp
        src/dialogs/NTSettingsDialog.cpp
        src/dialogs/WidgetDialogGenerator.cpp
        src/dialogs/TabMaxSizeDialog.cpp
        src/dialogs/PreferencesDialog.cpp

        # =======================================

        include/Globals.h
        include/MainWindow.h
        include/Constants.h

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
        include/widgets/FMSInfoWidget.h
        include/widgets/SwerveWidget.h

        include/misc/ShapedFrame.h
        include/misc/BetterDial.h
        include/misc/FieldImage.h
        include/misc/ChartView.h
        include/misc/GridLineWidget.h
        include/misc/SwerveTrain.h

        include/stores/TopicStore.h
        include/stores/TypeStore.h
        include/stores/FilterStore.h
        include/stores/CameraStore.h

        include/dialogs/CameraSelectionDialog.h
        include/dialogs/NewWidgetTreeDialog.h
        include/dialogs/NTSettingsDialog.h
        include/dialogs/WidgetDialogGenerator.h
        include/dialogs/TabMaxSizeDialog.h
        include/dialogs/PreferencesDialog.h

        # ==================================
        ui/MainWindow.ui
        ui/NTSettingsDialog.ui
        ui/WidgetDialogGenerator.ui
        ui/NewWidgetTreeDialog.ui
        ui/CameraSelectionDialog.ui
        ui/TabMaxSizeDialog.ui
        ui/PreferencesDialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(QFRCDashboardLib
        STATIC
        fields.qrc
        theme/qrc/breeze.qrc
        icons.qrc
        ${PROJECT_SOURCES}

    )
endif()

add_subdirectory("buildconfig")

target_link_libraries(QFRCDashboardLib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Charts ntcore BuildConfig)

target_include_directories(QFRCDashboardLib PRIVATE "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
target_include_directories(QFRCDashboardLib PRIVATE ui/)
target_include_directories(QFRCDashboardLib PRIVATE include/)

set(_Dashboard_AUTOGEN "${CMAKE_CURRENT_BINARY_DIR}/QFRCDashboardLib_autogen" PARENT_SCOPE)

include(GNUInstallDirs)
install(TARGETS QFRCDashboardLib
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/qt.conf\" \" \")"
    COMPONENT Runtime
)
