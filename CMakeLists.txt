cmake_minimum_required(VERSION 3.16)

set(QDASH_NAME QDash)
set(QDASH_ORG "Q-FRC")
set(QDASH_REPO https://github.com/${QDASH_ORG}/${QDASH_NAME})

project(${QDASH_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTORCC ON)

include(CMakeDependentOption)

option(QDASH_WEBVIEW OFF "Enable web view widget (Requires QtWebEngine)")
option(QDASH_CAMVIEW ON "Enable camera view widget (Requires QtMultimedia)")
cmake_dependent_option(QDASH_NETWORK "Enable remote layout feature (Requires QtNetwork)" ON "NOT QDASH_WEBVIEW" ON)

set(QDASH_QT_COMPONENTS Core Quick QuickControls2)
set(QDASH_QT_LIBRARIES Qt6::Quick Qt6::QuickControls2)

if (QDASH_WEBVIEW)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} WebEngineQuick)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::WebEngineQuick)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_WEBVIEW)
endif()

if (QDASH_CAMVIEW)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} Multimedia)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::Multimedia)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_CAMVIEW)
endif()

if (QDASH_NETWORK)
    set(QDASH_QT_COMPONENTS ${QDASH_QT_COMPONENTS} Network)
    set(QDASH_QT_LIBRARIES ${QDASH_QT_LIBRARIES} Qt6::Network)
    set(QDASH_DEFINES ${QDASH_DEFINES} QDASH_NETWORK)
endif()

find_package(Qt6 6.7 REQUIRED COMPONENTS ${QDASH_QT_COMPONENTS})

qt_standard_project_setup(REQUIRES 6.7)

if(WIN32)
    set(RCS_FILE ${CMAKE_CURRENT_BINARY_DIR}/dist/${QDASH_NAME}.rc)
    set(QDASH_BUILD_PLATFORM "win32")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    set(QDASH_BUILD_PLATFORM "lin64")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Git
include(GetGitRevisionDescription)
git_get_exact_tag(QDASH_TAG)
get_git_head_revision(QDASH_REFSPEC QDASH_COMMIT)

message(STATUS "Git commit: ${QDASH_COMMIT}")
message(STATUS "Git tag: ${QDASH_TAG}")
message(STATUS "Git refspec: ${QDASH_REFSPEC}")

string(REPLACE "-beta" "." QDASH_NUMERIC_TAG ${QDASH_TAG})
string(REPLACE "-rc" "." QDASH_NUMERIC_TAG ${QDASH_NUMERIC_TAG})

string(REPLACE "." " " QDASH_SPLIT_VERSION ${QDASH_NUMERIC_TAG})

separate_arguments(QDASH_SPLIT_VERSION)

list(GET QDASH_SPLIT_VERSION 0 QDASH_VERSION_MAJOR)
list(GET QDASH_SPLIT_VERSION 1 QDASH_VERSION_MINOR)
list(GET QDASH_SPLIT_VERSION 2 QDASH_VERSION_HOTFIX)

project(${QDASH_NAME} VERSION ${QDASH_NUMERIC_TAG} LANGUAGES CXX)

# Build Date
string(TIMESTAMP TODAY "%Y-%m-%d")
set(QDASH_BUILD_TIMESTAMP "${TODAY}")

add_subdirectory(dist)
add_subdirectory(ntcore EXCLUDE_FROM_ALL)
find_package(ntcore)

set(QT_QML_GENERATE_QMLLS_INI ON)

add_subdirectory(src)
