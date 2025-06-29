cmake_minimum_required(VERSION 3.25)
include(FetchContent)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)

set(FETCHCONTENT_QUIET OFF)

# WPILib
set(USE_SYSTEM_EIGEN OFF)
set(USE_SYSTEM_FMTLIB OFF)
set(WITH_JAVA OFF)

set(WITH_CSCORE OFF)
set(WITH_NTCORE ON)

set(WITH_WPIMATH OFF)
set(WITH_WPILIB OFF)
set(WITH_TESTS OFF)
set(WITH_GUI OFF)

set(WITH_PROTOBUF OFF)
set(WITH_SIMULATION_MODULES OFF)

set(NO_WERROR ON)

FetchContent_Declare(
        ntcore
        GIT_REPOSITORY https://github.com/wpilibsuite/allwpilib.git
        GIT_TAG        13626063dc590389366891635f2abbfedf7552cb # 2025.3.1
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(ntcore)
