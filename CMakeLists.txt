cmake_minimum_required(VERSION 3.5)

project(QFRCDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets MultimediaWidgets)
find_package(wpilib REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/Globals.cpp
        src/MainWindow.cpp

        src/widgets/BaseWidget.cpp
        src/widgets/NumberDisplayWidget.cpp
        src/widgets/TabWidget.cpp
        src/widgets/BooleanDisplayWidget.cpp
        src/widgets/StringDisplayWidget.cpp
        src/widgets/TextWidget.cpp
        src/widgets/BooleanCheckboxWidget.cpp
        src/widgets/DoubleDialWidget.cpp
        src/widgets/StringChooserWidget.cpp
        src/widgets/CameraViewWidget.cpp

        src/dialogs/NewWidgetDialog.cpp
        src/dialogs/NewBooleanCheckboxDialog.cpp
        src/dialogs/NewStringDisplayDialog.cpp
        src/dialogs/NewDoubleDisplayDialog.cpp
        src/dialogs/NewBooleanDisplayDialog.cpp
        src/dialogs/NewDoubleDialDialog.cpp
        src/dialogs/NewStringChooserDialog.cpp
        src/dialogs/NewCameraViewDialog.cpp

        src/misc/NewWidgetListDialog.cpp
        src/misc/ResizeDialog.cpp


        # =======================================

        include/Globals.h
        include/MainWindow.h

        include/widgets/BaseWidget.h
        include/widgets/NumberDisplayWidget.h
        include/widgets/TabWidget.h
        include/widgets/BooleanDisplayWidget.h
        include/widgets/StringDisplayWidget.h
        include/widgets/TextWidget.h
        include/widgets/BooleanCheckboxWidget.h
        include/widgets/DoubleDialWidget.h
        include/widgets/StringChooserWidget.h
        include/widgets/CameraViewWidget.h

        include/dialogs/NewWidgetDialog.h
        include/dialogs/NewBooleanCheckboxDialog.h
        include/dialogs/NewStringDisplayDialog.h
        include/dialogs/NewDoubleDisplayDialog.h
        include/dialogs/NewBooleanDisplayDialog.h
        include/dialogs/NewDoubleDialDialog.h
        include/dialogs/NewStringChooserDialog.h
        include/dialogs/NewCameraViewDialog.h

        include/misc/NewWidgetListDialog.h
        include/misc/ResizeDialog.h


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QFRCDashboard
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(QFRCDashboard PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets ntcore)

target_include_directories(QFRCDashboard PRIVATE include/)

include(GNUInstallDirs)
install(TARGETS QFRCDashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QFRCDashboard)
endif()
