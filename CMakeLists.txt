cmake_minimum_required(VERSION 3.16)

set(Dashboard_APP_NAME QFRCDashboard)
set(Dashboard_EXEC_NAME Dashboard)
set(Dashboard_ORG_NAME "Q-FRC")
set(Dashboard_GIT_REPO https://github.com/${Dashboard_ORG_NAME}/${Dashboard_EXEC_NAME})

project(QFRCDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTORCC ON)

option(USE_SYSTEM_NTCORE OFF "Use the system NTCore (Linux only)")

find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Multimedia QuickControls2 Network)
add_subdirectory(3rd_party EXCLUDE_FROM_ALL)

qt_standard_project_setup(REQUIRES 6.7)

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
string(REPLACE "-rc" "." Dashboard_GIT_TAG_NUMERO ${Dashboard_GIT_TAG_NUMERO})

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
find_package(ntcore)
find_package(Protobuf)

set(QRC_FILES
    fields.qrc
    accents.qrc
    icons.qrc)

qt_add_executable(${Dashboard_EXEC_NAME}
    main.cpp
    ${QRC_FILES}
    ${RCS_FILE}
)

set_source_files_properties(qml/Constants.qml
    PROPERTIES
    QT_QML_SINGLETON_TYPE true
)

set(QML_ELEMENTS
    Main
    Constants

    items/MainScreen
    items/Tab
    items/TopicView
    items/TitleBar
    items/IconButton
    items/RadialGauge

    widgets/BaseWidget
    widgets/ResizeAnchor
    widgets/DoubleSpinBox

    widgets/primitive/TextWidget
    widgets/primitive/BoolWidget
    widgets/primitive/EnumWidget
    widgets/primitive/ColorWidget
    widgets/primitive/ReefDisplay
    widgets/primitive/StringDisplay

    widgets/primitive/ShapeHandler

    widgets/primitive/number/IntWidget
    widgets/primitive/number/IntDisplay
    widgets/primitive/number/IntGaugeWidget
    widgets/primitive/number/IntDialWidget

    widgets/primitive/number/DoubleWidget
    widgets/primitive/number/DoubleDisplay
    widgets/primitive/number/MatchTime
    widgets/primitive/number/DoubleProgressBar
    widgets/primitive/number/DoubleGaugeWidget
    widgets/primitive/number/DoubleDialWidget

    widgets/sendable/FMSInfo
    widgets/sendable/Field2d
    widgets/sendable/Command
    widgets/sendable/StringChooser

    widgets/misc/CameraView
    widgets/misc/ErrorsWidget

    widgets/config/BaseConfigDialog
    widgets/config/LabeledTextField
    widgets/config/LabeledCheckbox
    widgets/config/LabeledSpinBox
    widgets/config/LabeledDoubleSpinBox
    widgets/config/LabeledComboBox
    widgets/config/TitleComponent
    widgets/config/ColorTable
    widgets/config/ColorField
    widgets/config/SectionHeader

    dialogs/tab/TabNameDialog
    dialogs/tab/TabDialog
    dialogs/tab/TabCloseDialog

    dialogs/remote/RemoteLayoutsDialog
    dialogs/remote/RemoteLayout

    dialogs/settings/ServerTab
    dialogs/settings/MiscTab
    dialogs/settings/WindowTab
    dialogs/settings/AppearanceTab
    dialogs/settings/SettingsDialog
    dialogs/settings/SettingsTabButton
    dialogs/settings/AppearanceComboBox

    dialogs/accent/AccentEditor
    dialogs/accent/AccentDisplay

    dialogs/AboutDialog
    dialogs/NotificationPopup
    dialogs/AnimatedDialog

    fields/BetterDoubleSpinBox
    fields/BetterSpinBox
    fields/BetterTextField
    fields/FieldFooter
    fields/BetterComboBox
    fields/BetterMenuItem
)

set(ELEMENTS
    Globals
    Constants
    SettingsManager
    ConnManager
    PlatformHelper
    NotificationHelper
    MaskedMouseArea

    models/TabWidgetsModel
    models/TabListModel
    models/TopicListModel
    models/MapModel
    models/AccentsListModel
    models/RemoteLayoutModel

    stores/TopicStore
)

foreach(ELEMENT ${ELEMENTS})
    set(HEADERS ${HEADERS} include/${ELEMENT}.h)
    set(SOURCES ${SOURCES} src/${ELEMENT}.cpp)
endforeach()

foreach(QML ${QML_ELEMENTS})
    set(QML_FILES ${QML_FILES} qml/${QML}.qml)
endforeach()

set(HEADERS
    ${HEADERS}
    include/Flags.h
    include/MetaObjectHelper.h
)

add_subdirectory(buildconfig)

qt_add_qml_module(${Dashboard_EXEC_NAME}
    URI QFRCDashboard
    VERSION 1.0
    QML_FILES ${QML_FILES}

    SOURCES ${SOURCES} ${HEADERS}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${Dashboard_EXEC_NAME} PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${Dashboard_EXEC_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${Dashboard_EXEC_NAME}
    PRIVATE Qt6::Quick
    Qt6::Multimedia
    Qt6::QuickControls2
    Qt6::Network
    BuildConfig
    ntcore
)

target_include_directories(${Dashboard_EXEC_NAME} PRIVATE include/)
target_include_directories(${Dashboard_EXEC_NAME} PRIVATE include/models)
target_include_directories(${Dashboard_EXEC_NAME} PRIVATE include/stores)
target_include_directories(${Dashboard_EXEC_NAME} PRIVATE "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")

include(GNUInstallDirs)
install(TARGETS ${Dashboard_EXEC_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(${Dashboard_EXEC_NAME} PROPERTIES
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
