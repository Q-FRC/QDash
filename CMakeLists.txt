cmake_minimum_required(VERSION 3.12)

project(QFRCDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets MultimediaWidgets)
find_package(ntcore REQUIRED)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

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

        src/misc/NewWidgetTreeDialog.cpp
        src/misc/NTSettingsDialog.cpp
        src/misc/WidgetDialogGenerator.cpp
        src/misc/ShapedFrame.cpp
        src/misc/BetterDial.cpp
        src/misc/FieldImage.cpp
        src/misc/TabMaxSizeDialog.cpp

        src/stores/TopicStore.cpp
        src/stores/TypeStore.cpp
        src/stores/FilterStore.cpp

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

        include/misc/NewWidgetTreeDialog.h
        include/misc/NTSettingsDialog.h
        include/misc/WidgetDialogGenerator.h
        include/misc/ShapedFrame.h
        include/misc/BetterDial.h
        include/misc/FieldImage.h
        include/misc/TabMaxSizeDialog.h

        include/stores/TopicStore.h
        include/stores/TypeStore.h
        include/stores/FilterStore.h
)

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/QFRCDashboard.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QFRCDashboard
        MANUAL_FINALIZATION
        fields.qrc
        ${RCS_FILE}
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

set(Dashboard_VERSION "1.0.0-beta4")
set(Dashboard_VERSION_NUMBER "1.0.0.4")

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QFRCDashboard)
endif()

if(UNIX AND NOT(APPLE))
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QFRCDashboard.desktop DESTINATION "share/applications")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QFRCDashboard.svg DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QFRCDashboard-symbolic.svg DESTINATION "share/icons/hicolor/symbolic/apps" RENAME QFRCDashboard.svg)
endif()

configure_file(QFRCDashboard.rc.in QFRCDashboard.rc)
configure_file(QFRCDashboard.manifest.in QFRCDashboard.manifest)
configure_file(QFRCDashboard.ico QFRCDashboard.ico COPYONLY)
